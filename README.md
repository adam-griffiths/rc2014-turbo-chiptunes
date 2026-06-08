# rc2014-turbo-chiptunes
Turbo Pascal 3.0 INC file with Procedures and Helpers for the YM/AY Sound Card targeted at the YM2149 Chip

Good Evening Mr Dillinger...

YM2149.INC Documentation
========================

Overview
--------
YM2149.INC is a Turbo Pascal include file for controlling a YM2149 / AY-3-8910
style PSG sound chip through I/O ports.

It provides:
- direct register writes
- tone generation on channels A, B, and C
- volume control
- envelope control
- noise control
- delay/timing helpers
- note-name to period conversion
- convenience procedures for simple music playback

The include is procedural, not object-oriented. Some routines are intended as
public API for application code, while others are best treated as internal
helpers.

Terminology
-----------
Public:
Procedures/functions intended to be called directly by your programs.

Internal:
Procedures/functions mainly used by other routines inside YM2149.INC.
You can call them if you want, but they are lower-level building blocks.

General Notes
-------------
1. The YM2149 has three tone channels:
   - Channel A
   - Channel B
   - Channel C

2. Pitch is controlled by a tone period value.
   Larger period = lower note.
   Smaller period = higher note.

3. Volume can be:
   - fixed (0..15)
   - envelope-controlled

4. The mixer register controls whether tone and/or noise are enabled for each
   channel.

5. The envelope generator is global to the chip.
   Multiple channels may use it at once, but they share the same envelope
   period and shape.

6. Period constants in this file are tuned to the target hardware setup.
   If chip clocking changes, pitch tables may need recalculation.

Constants
=========

Port Constants
--------------
YMRegPort
  Register select port.

YMDataPort
  Data write port.

These are hardware-specific and depend on how the chip is connected.


Channel Constants
-----------------
YMChanA = 0
YMChanB = 1
YMChanC = 2

These are the three PSG tone channels.


Suggested Role Constants
------------------------
YMLeadChan
YMHarmonyChan
YMBassChan

These are convenience aliases assigning musical roles to channels.
They do not change hardware behavior; they just make calling code easier to
read.


Accidental Constants
--------------------
AccNatural = ' '
AccSharp   = '#'
AccFlat    = 'B'

These are used by YMGetPeriod and note-playing routines.

Important:
AccFlat uses the character 'B', not a lowercase or special flat symbol.


Rest Constant
-------------
N_REST = 0

Used to represent silence/rest in note playback routines.


Length Constants
----------------
NLWhole
NLHalf
NLQuarter
NLEighth
NLSixteenth

These are timing units used with YMDelayUnits and note playback helpers.
Their actual duration depends on YMTempo.


Note Period Constants
---------------------
Examples:
N_C3, N_CS3, ... N_B3
N_C4, N_CS4, ... N_B4
...
N_C9, N_CS9, ... N_B9

These are chip tone period values for specific notes.

Rules:
- lower octave = larger period
- higher octave = smaller period

Practical notes:
- very low notes may exceed chip period limits
- very high notes may sound thin or harsh


=========
Variables
=========

YMMixerReg
----------
Internal cached copy of mixer register 7.

Internal use:
Used so tone/noise enable bits can be changed without reconstructing the whole
register value every time.

Treat as internal.


YMTempo
-------
Delay scaling factor used by YMDelayUnits.

Publicly affected by:
YMSetTempo

Normally you should use YMSetTempo rather than assigning YMTempo directly.

Procedure and Function Reference
================================

Low-Level Hardware Access
-------------------------

YMWrite(RegNum, Value)
Public, but low-level.

Purpose:
Writes Value to chip register RegNum.

Use when:
- you want direct hardware control
- you know the YM2149 register map
- you need a feature not wrapped by a helper routine

Notes:
This is the fundamental register-write routine used by almost everything else
in the include.

Typical application code:
Usually not needed unless doing advanced work.


YMSetMixerValue(M)
Internal / advanced public.

Purpose:
Stores a masked copy of mixer register bits and writes them to register 7.

Use when:
- you want direct control over tone/noise enable bits for all channels at once

Notes:
Only the low 6 bits are relevant here.

Normally higher-level routines such as YMEnableTone, YMDisableTone,
YMEnableNoise, and YMDisableNoise are easier to use.


Tone and Noise Control
----------------------

YMEnableTone(Channel)
Public.

Purpose:
Enables tone output on the specified channel.

Channels:
YMChanA, YMChanB, YMChanC

Typical use:
Call before or during note playback if you want the tone generator audible.


YMDisableTone(Channel)
Public.

Purpose:
Disables tone output on the specified channel.

Typical use:
Useful for noise-only effects or direct mixer experimentation.


YMEnableNoise(Channel)
Public.

Purpose:
Enables noise output on the specified channel.


YMDisableNoise(Channel)
Public.

Purpose:
Disables noise output on the specified channel.

Typical note playback:
Most melodic note helpers disable noise and enable tone automatically.


Pitch and Volume
----------------

YMSetTone(Channel, Period)
Public.

Purpose:
Sets the tone period for the specified channel.

Arguments:
Channel - A, B, or C
Period  - tone period value

Meaning:
- higher Period => lower note
- lower Period => higher note

Notes:
This does not automatically set volume or start/stop timing.
It only sets the tone registers.


YMSetVolume(Channel, Volume)
Public.

Purpose:
Sets fixed volume on a channel.

Volume range:
0..15

Behavior:
0 = silent
15 = loudest fixed volume

Important:
This uses fixed volume mode, not envelope mode.


YMUseEnvelope(Channel)
Public.

Purpose:
Makes a channel use the hardware envelope instead of fixed volume.

How:
Writes 16 to the channel volume register.

Important:
The envelope shape and speed must already be configured globally using
YMSetEnvPeriod and YMSetEnvShape.


YMUseFixedVolume(Channel, Volume)
Public.

Purpose:
Convenience wrapper to switch a channel back to fixed volume mode and apply
a volume level.

Equivalent idea:
Calls YMSetVolume(Channel, Volume).


Noise and Envelope Setup
------------------------

YMNoisePeriod(Period)
Public.

Purpose:
Sets the noise generator period.

Range:
The chip uses only the low 5 bits.

Use when:
- generating percussion/noise effects
- combining noise with tone


YMSetEnvPeriod(Period)
Public.

Purpose:
Sets the envelope period.

Meaning:
Controls how fast the envelope advances.

Behavior:
- larger period = slower envelope
- smaller period = faster envelope

Registers used:
R11 and R12


YMSetEnvShape(Shape)
Public.

Purpose:
Sets the envelope shape.

Valid values:
0..15

Registers used:
R13

Important behavior:
Writing R13 restarts/retriggers the envelope sequence.

This is useful if you want each note to start at the beginning of the envelope.


Global Control
--------------

YMAllOff
Public.

Purpose:
Silences all three channels by setting volumes to 0.

Use when:
- stopping playback
- cleanup before exit
- ending a chord or phrase


YMInit
Public.

Purpose:
Initializes the chip and internal state for normal tone playback.

What it does:
- resets mixer cache
- sets mixer to a tone-friendly state
- clears/sets noise defaults
- initializes envelope period and shape
- silences channels
- sets default tempo

Use:
Call once before using most other sound routines.


Timing Helpers
--------------

YMRawDelay(Count)
Internal / advanced public.

Purpose:
Performs a crude software delay loop.

Notes:
This is machine-speed dependent and not musically exact.
It is used internally by YMDelayUnits.

Normally:
Use YMDelayUnits instead of calling this directly.


YMSetTempo(T)
Public.

Purpose:
Sets the delay scaling factor used for note durations.

Behavior:
Higher values generally produce longer delays.

Typical use:
Adjust song playback speed.


YMDelayUnits(Units)
Public.

Purpose:
Delays for a number of note-length units.

How:
Uses Units * YMTempo through YMRawDelay.

Use:
- between events
- for manual timing
- used internally by note playback helpers


Note Start/Stop Routines
------------------------

YMNoteOn(Channel, Period, Volume)
Public.

Purpose:
Starts a fixed-volume note immediately.

What it does:
- sets tone period
- enables tone
- disables noise
- applies fixed volume

Important:
This does not stop automatically.
The note continues until you change or stop it.


YMEnvNoteOn(Channel, Period)
Public.

Purpose:
Starts a note using the hardware envelope.

What it does:
- sets tone period
- enables tone
- disables noise
- enables envelope volume mode

Important:
Envelope period and shape must be configured already.


YMNoteOff(Channel)
Public.

Purpose:
Stops a channel by setting its volume to 0.


YMStartNote(Channel, Period, Volume)
Public.

Purpose:
Alias/convenience wrapper for YMNoteOn.

Notes:
Functionally similar to YMNoteOn.
Useful when naming code in a musical way.


YMStartEnvNote(Channel, Period)
Public.

Purpose:
Alias/convenience wrapper for YMEnvNoteOn.


YMStopNote(Channel)
Public.

Purpose:
Alias/convenience wrapper for YMNoteOff.


Timed Playback Routines
-----------------------

YMPlayPeriod(Channel, Period, Volume, Length)
Public.

Purpose:
Plays a fixed-volume tone period for a duration, then stops it.

Behavior:
- if Period = N_REST, produces silence for Length
- otherwise starts note, waits, then stops note

Use:
Good for simple monophonic playback.


YMPlayEnvPeriod(Channel, Period, Length)
Public.

Purpose:
Plays a note using envelope volume for a duration, then stops it.

Behavior:
- if Period = N_REST, produces silence
- otherwise starts envelope-driven note, waits, then stops it


Pitch Conversion
----------------

YMGetPeriod(NoteName, Accidental, Octave) : integer
Public.

Purpose:
Converts symbolic note information into a PSG tone period constant.

Arguments:
NoteName   - 'A'..'G'
Accidental - AccNatural, AccSharp, AccFlat
Octave     - supported octave number

Returns:
- matching note period constant
- 0 if no matching note is found

Examples:
YMGetPeriod('C', AccNatural, 4)
YMGetPeriod('F', AccSharp, 6)
YMGetPeriod('B', AccFlat, 5)

Important:
If you add new octave constants, this function must also be extended to use
them.


Named Note Playback
-------------------

YMPlayNote(Channel, NoteName, Accidental, Octave, Volume, Length)
Public.

Purpose:
Plays a note by name at fixed volume for a duration.

How:
- converts note to period using YMGetPeriod
- calls YMPlayPeriod


YMPlayEnvNote(Channel, NoteName, Accidental, Octave, Length)
Public.

Purpose:
Plays a note by name using the hardware envelope.


YMRest(Channel, Length)
Public.

Purpose:
Silences the channel for a duration.

Use:
Insert rests into melodies or multi-channel arrangements.


Channel Role Helpers
--------------------

YMPlayLead(NoteName, Accidental, Octave, Volume, Length)
Public.

Purpose:
Plays a note on YMLeadChan.

Use:
Convenience for melody lines.


YMPlayHarmony(NoteName, Accidental, Octave, Volume, Length)
Public.

Purpose:
Plays a note on YMHarmonyChan.


YMPlayBass(NoteName, Accidental, Octave, Volume, Length)
Public.

Purpose:
Plays a note on YMBassChan.


Chord Helpers
-------------

YMPlayMajorTriad(RootName, RootAcc, Octave, Volume, Length)
Public.

Purpose:
Plays selected major triads using the three hardware channels.

Current supported roots in the include:
- C major
- F major
- G major

Behavior:
- assigns one chord tone to each channel
- plays for Length
- silences all channels after

Limitations:
This is not a fully generic chord engine.
Only coded roots are supported.


YMPlayMinorTriad(RootName, RootAcc, Octave, Volume, Length)
Public.

Purpose:
Plays selected minor triads using the three hardware channels.

Current supported roots:
- A minor
- D minor
- E minor

Behavior:
Same idea as YMPlayMajorTriad.

Suggested Public API vs Internal Helpers
========================================

Recommended Public API
----------------------
These are the routines most application code should use:

YMInit
YMSetTempo
YMSetEnvPeriod
YMSetEnvShape
YMNoisePeriod

YMSetTone
YMSetVolume
YMUseEnvelope
YMUseFixedVolume

YMNoteOn
YMEnvNoteOn
YMNoteOff
YMStartNote
YMStartEnvNote
YMStopNote

YMPlayPeriod
YMPlayEnvPeriod
YMGetPeriod
YMPlayNote
YMPlayEnvNote
YMRest

YMPlayLead
YMPlayHarmony
YMPlayBass

YMPlayMajorTriad
YMPlayMinorTriad

YMAllOff


Mostly Internal Helpers
-----------------------
These are primarily building blocks used by the higher-level routines:

YMWrite
YMSetMixerValue
YMRawDelay

These are still callable, but they are lower-level and easier to misuse.


Mixed-Level Helpers
-------------------
These are valid public calls, but a little closer to hardware detail:

YMEnableTone
YMDisableTone
YMEnableNoise
YMDisableNoise
YMDelayUnits

Use them when you want more manual control.

Envelope Shapes Reference
=========================

The envelope shape is written to register 13.
Only values 0..15 are valid.

Bit meanings:
- bit 3 = Continue
- bit 2 = Attack
- bit 1 = Alternate
- bit 0 = Hold

Practical shape table:

0  = 0000  \________   single decay, then remain low
1  = 0001  same as 0
2  = 0010  same as 0
3  = 0011  same as 0

4  = 0100  /^^^^^^^^   single rise, then end
5  = 0101  same as 4
6  = 0110  same as 4
7  = 0111  same as 4

8  = 1000  \\\\\\\\\   repeating decay
9  = 1001  \________   decay once, hold low
10 = 1010  \/\/\/\/\   repeating triangle
11 = 1011  \^^^^^^^^   decay, then hold high

12 = 1100  /////////   repeating rise
13 = 1101  /^^^^^^^^   rise once, hold high
14 = 1110  /\/\/\/\/   inverted triangle
15 = 1111  /________   rise, then hold low

Common useful choices:
9   fade out
10  repeating triangle
12  repeating ramp up
13  attack then hold high

Important:
Writing YMSetEnvShape(...) retriggers the envelope.

Typical Usage Patterns
======================

1. Basic fixed-volume note
--------------------------
Call:
- YMInit
- YMSetTempo(...)
- YMPlayNote(...)

Example idea:
YMInit;
YMSetTempo(12);
YMPlayNote(YMChanA, 'C', AccNatural, 4, 10, NLQuarter);


2. Sustained note started and stopped manually
----------------------------------------------
Call:
- YMStartNote(...)
- YMDelayUnits(...)
- YMStopNote(...)

Example idea:
YMStartNote(YMChanA, YMGetPeriod('A', AccNatural, 4), 12);
YMDelayUnits(NLHalf);
YMStopNote(YMChanA);


3. Envelope-driven note
-----------------------
Call:
- YMSetEnvPeriod(...)
- YMSetEnvShape(...)
- YMPlayEnvNote(...)

Example idea:
YMSetEnvPeriod(256);
YMSetEnvShape(10);
YMPlayEnvNote(YMChanA, 'C', AccNatural, 5, NLQuarter);


4. Chord playback
-----------------
Call:
- YMPlayMajorTriad(...)
or
- YMPlayMinorTriad(...)

Example idea:
YMPlayMajorTriad('C', AccNatural, 4, 10, NLHalf);

Cautions and Limits
===================

1. Envelope generator is shared globally.
   All channels using envelope mode share the same envelope shape and speed.

2. YMRawDelay is CPU-speed dependent.
   Timing is approximate, not absolute.

3. Very low note periods may exceed hardware limits.
   Very high note periods may become shrill or less musical.

4. YMSetTone only sets pitch.
   It does not ensure the note is audible unless tone is enabled and volume or
   envelope is active.

5. YMNoteOff works by setting volume to 0.
   It does not disable tone generation bits in the mixer.

Recommended Development Style
=============================

- Treat YMInit as required startup.
- Use YMGetPeriod + YMPlayNote for simple melodies.
- Use YMStartNote / YMStopNote for overlapping or sustained notes.
- Use YMSetEnvPeriod + YMSetEnvShape + YMUseEnvelope for more expressive sounds.
- Use higher-level routines unless you specifically need register-level control.
- Extend new octaves by:
  1. adding note constants
  2. extending YMGetPeriod
  and changing nothing else unless necessary


End of line...
