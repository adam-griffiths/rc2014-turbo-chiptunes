const
  { I/O ports for the YM2149 board }
  YMRegPort = 216;
  YMDataPort = 208;

  { Channel numbers }
  YMChanA = 0;
  YMChanB = 1;
  YMChanC = 2;

  { Suggested musical roles.
  Actual stereo image depends on the RC2014 board mixer hardware. }
  YMLeadChan = YMChanA;
  YMHarmonyChan = YMChanB;
  YMBassChan = YMChanC;

  { Accidental markers }
  AccNatural = ' ';
  AccSharp = '#';
  AccFlat = 'B';

  { Rest value }
  N_REST = 0;

  { Basic note lengths }
  NLWhole = 64;
  NLHalf = 32;
  NLQuarter = 16;
  NLEighth = 8;
  NLSixteenth = 4;

  { Octave 3 note periods - Added Manually}
  N_C3 = 7644;
  N_CS3 = 7216;
  N_D3 = 6810;
  N_DS3 = 6428;
  N_E3 = 6068;
  N_F3 = 5726;
  N_FS3 = 5406;
  N_G3 = 5102;
  N_GS3 = 4816;
  N_A3 = 4546;
  N_AS3 = 4290;
  N_B3 = 4050;
  { Octave 4 note periods }
  N_C4 = 3822;
  N_CS4 = 3608;
  N_D4 = 3405;
  N_DS4 = 3214;
  N_E4 = 3034;
  N_F4 = 2863;
  N_FS4 = 2703;
  N_G4 = 2551;
  N_GS4 = 2408;
  N_A4 = 2273;
  N_AS4 = 2145;
  N_B4 = 2025;

  { Octave 5 note periods }
  N_C5 = 1911;
  N_CS5 = 1804;
  N_D5 = 1703;
  N_DS5 = 1607;
  N_E5 = 1517;
  N_F5 = 1432;
  N_FS5 = 1351;
  N_G5 = 1276;
  N_GS5 = 1204;
  N_A5 = 1136;
  N_AS5 = 1073;
  N_B5 = 1012;

  { Octave 6 note periods }
  N_C6 = 956;
  N_CS6 = 902;
  N_D6 = 851;
  N_DS6 = 804;
  N_E6 = 758;
  N_F6 = 716;
  N_FS6 = 676;
  N_G6 = 638;
  N_GS6 = 602;
  N_A6 = 568;
  N_AS6 = 536;
  N_B6 = 506;

  { Octave 7 note periods - added manually}
  N_C7 = 478;
  N_CS7 = 451;
  N_D7 = 426;
  N_DS7 = 402;
  N_E7 = 379;
  N_F7 = 358;
  N_FS7 = 338;
  N_G7 = 319;
  N_GS7 = 301;
  N_A7 = 284;
  N_AS7 = 268;
  N_B7 = 253;

var
  { Cached mixer register and delay scaling }
  YMMixerReg : integer;
  YMTempo : integer;

{ Write a value to a YM2149 register }
procedure YMWrite(RegNum, Value : integer);
begin
  Port[YMRegPort] := RegNum;
  Port[YMDataPort] := Value;
end;

{ Set register 7 mixer value and cache it }
procedure YMSetMixerValue(M : integer);
begin
  YMMixerReg := M and 63;
  YMWrite(7, YMMixerReg);
end;

{ Enable tone output for one channel }
procedure YMEnableTone(Channel : integer);
begin
  case Channel of
    0: YMMixerReg := YMMixerReg and 62;
    1: YMMixerReg := YMMixerReg and 61;
    2: YMMixerReg := YMMixerReg and 59;
  end;
  YMWrite(7, YMMixerReg);
end;

{ Disable tone output for one channel }
procedure YMDisableTone(Channel : integer);
begin
  case Channel of
    0: YMMixerReg := YMMixerReg or 1;
    1: YMMixerReg := YMMixerReg or 2;
    2: YMMixerReg := YMMixerReg or 4;
  end;
  YMWrite(7, YMMixerReg);
end;

{ Enable noise output for one channel }
procedure YMEnableNoise(Channel : integer);
begin
  case Channel of
    0: YMMixerReg := YMMixerReg and 55;
    1: YMMixerReg := YMMixerReg and 47;
    2: YMMixerReg := YMMixerReg and 31;
  end;
  YMWrite(7, YMMixerReg);
end;

{ Disable noise output for one channel }
procedure YMDisableNoise(Channel : integer);
begin
  case Channel of
    0: YMMixerReg := YMMixerReg or 8;
    1: YMMixerReg := YMMixerReg or 16;
    2: YMMixerReg := YMMixerReg or 32;
  end;
  YMWrite(7, YMMixerReg);
end;

{ Set tone period for channel A, B, or C }
procedure YMSetTone(Channel, Period : integer);
begin
  if Period < 1 then Period := 1;
    case Channel of
      0:
        begin
          YMWrite(0, Period mod 256);
          YMWrite(1, (Period div 256) mod 16);
        end;
      1:
        begin
          YMWrite(2, Period mod 256);
          YMWrite(3, (Period div 256) mod 16);
        end;
      2:
        begin
          YMWrite(4, Period mod 256);
          YMWrite(5, (Period div 256) mod 16);
        end;
    end;
end;

{ Set channel volume from 0 to 15 }
procedure YMSetVolume(Channel, Volume : integer);
begin
  if Volume < 0 then Volume := 0;
  if Volume > 15 then Volume := 15;
  case Channel of
    0: YMWrite(8, Volume);
    1: YMWrite(9, Volume);
    2: YMWrite(10, Volume);
  end;
end;

{ Enable hardware envelope on one channel }
procedure YMUseEnvelope(Channel : integer);
begin
  case Channel of
    0: YMWrite(8, 16);
    1: YMWrite(9, 16);
    2: YMWrite(10, 16);
   end;
end;

{ Use fixed volume on one channel }
procedure YMUseFixedVolume(Channel, Volume : integer);
begin
  YMSetVolume(Channel, Volume);
end;

{ Set noise generator period }
procedure YMNoisePeriod(Period : integer);
begin
  YMWrite(6, Period and 31);
end;

{ Set envelope period }
procedure YMSetEnvPeriod(Period : integer);
begin
  if Period < 1 then Period := 1;
  YMWrite(11, Period mod 256);
  YMWrite(12, (Period div 256) mod 256);
end;

{ Set envelope shape }
procedure YMSetEnvShape(Shape : integer);
begin
  YMWrite(13, Shape and 15);
end;

{ Silence all three channels }
procedure YMAllOff;
begin
  YMSetVolume(YMChanA, 0);
  YMSetVolume(YMChanB, 0);
  YMSetVolume(YMChanC, 0);
end;

{ Initialize chip for tone playback }
procedure YMInit;
begin
  YMMixerReg := 0;
  YMSetMixerValue(56);
  YMNoisePeriod(0);
  YMSetEnvPeriod(256);
  YMSetEnvShape(0);
  YMAllOff;
  YMTempo := 12;
end;

{ Crude software delay loop }
procedure YMRawDelay(Count : integer);
var
  I, J : integer;

begin
  for I := 1 to Count do
    for J := 1 to 200 do
      begin
      end;
end;

{ Set delay scaling factor }
procedure YMSetTempo(T : integer);
begin
  if T < 1 then T := 1;
  YMTempo := T;
end;

{ Delay by note-length units }
procedure YMDelayUnits(Units : integer);
begin
   YMRawDelay(Units * YMTempo);
end;

{ Turn on a note using raw period value and fixed volume }
procedure YMNoteOn(Channel, Period, Volume : integer);
begin
  YMSetTone(Channel, Period);
  YMEnableTone(Channel);
  YMDisableNoise(Channel);
  YMUseFixedVolume(Channel, Volume);
end;
{ Turn on a note using envelope volume }
procedure YMEnvNoteOn(Channel, Period : integer);
begin
  YMSetTone(Channel, Period);
  YMEnableTone(Channel);
  YMDisableNoise(Channel);
  YMUseEnvelope(Channel);
end;

{ Turn off one channel }
procedure YMNoteOff(Channel : integer);
begin
  YMSetVolume(Channel, 0);
end;

{ Start a fixed-volume note and leave it sounding }
procedure YMStartNote(Channel, Period, Volume : integer);
begin
  YMNoteOn(Channel, Period, Volume);
end;

{ Start an envelope note and leave it sounding }
procedure YMStartEnvNote(Channel, Period : integer);
begin
  YMEnvNoteOn(Channel, Period);
end;

{ Stop a sounding note }
procedure YMStopNote(Channel : integer);
begin
  YMNoteOff(Channel);
end;

{ Play a raw period for a duration, or rest if period is zero }
procedure YMPlayPeriod(Channel, Period, Volume, Length : integer);
begin
  if Period = N_REST then
    begin
      YMNoteOff(Channel);
      YMDelayUnits(Length);
    end
  else
    begin
      YMNoteOn(Channel, Period, Volume);
      YMDelayUnits(Length);
      YMNoteOff(Channel);
    end;
end;

{ Play a raw period using envelope volume }
procedure YMPlayEnvPeriod(Channel, Period, Length : integer);
begin
  if Period = N_REST then
    begin
      YMNoteOff(Channel);
      YMDelayUnits(Length);
    end
  else
    begin
      YMEnvNoteOn(Channel, Period);
      YMDelayUnits(Length);
      YMNoteOff(Channel);
    end;
end;

{ Convert note name, accidental, and octave to a YM period }
function YMGetPeriod(NoteName, Accidental : char;
  Octave : integer) : integer;
begin
YMGetPeriod := 0;

if Octave = 3 then
  begin
    if (NoteName = 'C') and
      (Accidental = AccNatural) then YMGetPeriod := N_C3;
    if (NoteName = 'C') and
      (Accidental = AccSharp) then YMGetPeriod := N_CS3;
    if (NoteName = 'D') and
      (Accidental = AccNatural) then YMGetPeriod := N_D3;
    if (NoteName = 'D') and
      (Accidental = AccSharp) then YMGetPeriod := N_DS3;
    if (NoteName = 'D') and
      (Accidental = AccFlat) then YMGetPeriod := N_CS3;
    if (NoteName = 'E') and
      (Accidental = AccNatural) then YMGetPeriod := N_E3;
    if (NoteName = 'E') and
      (Accidental = AccFlat) then YMGetPeriod := N_DS3;
    if (NoteName = 'F') and
      (Accidental = AccNatural) then YMGetPeriod := N_F3;
    if (NoteName = 'F') and
      (Accidental = AccSharp) then YMGetPeriod := N_FS3;
    if (NoteName = 'G') and
      (Accidental = AccNatural) then YMGetPeriod := N_G3;
    if (NoteName = 'G') and
      (Accidental = AccSharp) then YMGetPeriod := N_GS3;
    if (NoteName = 'G') and
      (Accidental = AccFlat) then YMGetPeriod := N_FS3;
    if (NoteName = 'A') and
      (Accidental = AccNatural) then YMGetPeriod := N_A3;
    if (NoteName = 'A') and
      (Accidental = AccSharp) then YMGetPeriod := N_AS3;
    if (NoteName = 'A') and
      (Accidental = AccFlat) then YMGetPeriod := N_GS3;
    if (NoteName = 'B') and
      (Accidental = AccNatural) then YMGetPeriod := N_B3;
    if (NoteName = 'B') and
      (Accidental = AccFlat) then YMGetPeriod := N_AS3;
    end;

  if Octave = 4 then
  begin
    if (NoteName = 'C') and
      (Accidental = AccNatural) then YMGetPeriod := N_C4;
    if (NoteName = 'C') and
      (Accidental = AccSharp) then YMGetPeriod := N_CS4;
    if (NoteName = 'D') and
      (Accidental = AccNatural) then YMGetPeriod := N_D4;
    if (NoteName = 'D') and
      (Accidental = AccSharp) then YMGetPeriod := N_DS4;
    if (NoteName = 'D') and
      (Accidental = AccFlat) then YMGetPeriod := N_CS4;
    if (NoteName = 'E') and
      (Accidental = AccNatural) then YMGetPeriod := N_E4;
    if (NoteName = 'E') and
      (Accidental = AccFlat) then YMGetPeriod := N_DS4;
    if (NoteName = 'F') and
      (Accidental = AccNatural) then YMGetPeriod := N_F4;
    if (NoteName = 'F') and
      (Accidental = AccSharp) then YMGetPeriod := N_FS4;
    if (NoteName = 'G') and
      (Accidental = AccNatural) then YMGetPeriod := N_G4;
    if (NoteName = 'G') and
      (Accidental = AccSharp) then YMGetPeriod := N_GS4;
    if (NoteName = 'G') and
      (Accidental = AccFlat) then YMGetPeriod := N_FS4;
    if (NoteName = 'A') and
      (Accidental = AccNatural) then YMGetPeriod := N_A4;
    if (NoteName = 'A') and
      (Accidental = AccSharp) then YMGetPeriod := N_AS4;
    if (NoteName = 'A') and
      (Accidental = AccFlat) then YMGetPeriod := N_GS4;
    if (NoteName = 'B') and
      (Accidental = AccNatural) then YMGetPeriod := N_B4;
    if (NoteName = 'B') and
      (Accidental = AccFlat) then YMGetPeriod := N_AS4;
  end;

  if Octave = 5 then
  begin
    if (NoteName = 'C') and
      (Accidental = AccNatural) then YMGetPeriod := N_C5;
    if (NoteName = 'C') and
      (Accidental = AccSharp) then YMGetPeriod := N_CS5;
    if (NoteName = 'D') and
      (Accidental = AccNatural) then YMGetPeriod := N_D5;
    if (NoteName = 'D') and
      (Accidental = AccSharp) then YMGetPeriod := N_DS5;
    if (NoteName = 'D') and
      (Accidental = AccFlat) then YMGetPeriod := N_CS5;
    if (NoteName = 'E') and
      (Accidental = AccNatural) then YMGetPeriod := N_E5;
    if (NoteName = 'E') and
      (Accidental = AccFlat) then YMGetPeriod := N_DS5;
    if (NoteName = 'F') and
      (Accidental = AccNatural) then YMGetPeriod := N_F5;
    if (NoteName = 'F') and
      (Accidental = AccSharp) then YMGetPeriod := N_FS5;
    if (NoteName = 'G') and
      (Accidental = AccNatural) then YMGetPeriod := N_G5;
    if (NoteName = 'G') and
      (Accidental = AccSharp) then YMGetPeriod := N_GS5;
    if (NoteName = 'G') and
      (Accidental = AccFlat) then YMGetPeriod := N_FS5;
    if (NoteName = 'A') and
      (Accidental = AccNatural) then YMGetPeriod := N_A5;
    if (NoteName = 'A') and
      (Accidental = AccSharp) then YMGetPeriod := N_AS5;
    if (NoteName = 'A') and
      (Accidental = AccFlat) then YMGetPeriod := N_GS5;
    if (NoteName = 'B') and
      (Accidental = AccNatural) then YMGetPeriod := N_B5;
    if (NoteName = 'B') and
      (Accidental = AccFlat) then YMGetPeriod := N_AS5;
  end;

  if Octave = 6 then
  begin
    if (NoteName = 'C') and
      (Accidental = AccNatural) then YMGetPeriod := N_C6;
    if (NoteName = 'C') and
      (Accidental = AccSharp) then YMGetPeriod := N_CS6;
    if (NoteName = 'D') and
      (Accidental = AccNatural) then YMGetPeriod := N_D6;
    if (NoteName = 'D') and
      (Accidental = AccSharp) then YMGetPeriod := N_DS6;
    if (NoteName = 'D') and
      (Accidental = AccFlat) then YMGetPeriod := N_CS6;
    if (NoteName = 'E') and
      (Accidental = AccNatural) then YMGetPeriod := N_E6;
    if (NoteName = 'E') and
      (Accidental = AccFlat) then YMGetPeriod := N_DS6;
    if (NoteName = 'F') and
      (Accidental = AccNatural) then YMGetPeriod := N_F6;
    if (NoteName = 'F') and
      (Accidental = AccSharp) then YMGetPeriod := N_FS6;
    if (NoteName = 'G') and
      (Accidental = AccNatural) then YMGetPeriod := N_G6;
    if (NoteName = 'G') and
      (Accidental = AccSharp) then YMGetPeriod := N_GS6;
    if (NoteName = 'G') and
      (Accidental = AccFlat) then YMGetPeriod := N_FS6;
    if (NoteName = 'A') and
      (Accidental = AccNatural) then YMGetPeriod := N_A6;
    if (NoteName = 'A') and
      (Accidental = AccSharp) then YMGetPeriod := N_AS6;
    if (NoteName = 'A') and
      (Accidental = AccFlat) then YMGetPeriod := N_GS6;
    if (NoteName = 'B') and
      (Accidental = AccNatural) then YMGetPeriod := N_B6;
    if (NoteName = 'B') and
      (Accidental = AccFlat) then YMGetPeriod := N_AS6;
  end;

  if Octave = 7 then
  begin
    if (NoteName = 'C') and
      (Accidental = AccNatural) then YMGetPeriod := N_C7;
    if (NoteName = 'C') and
      (Accidental = AccSharp) then YMGetPeriod := N_CS7;
    if (NoteName = 'D') and
      (Accidental = AccNatural) then YMGetPeriod := N_D7;
    if (NoteName = 'D') and
      (Accidental = AccSharp) then YMGetPeriod := N_DS7;
    if (NoteName = 'D') and
      (Accidental = AccFlat) then YMGetPeriod := N_CS7;
    if (NoteName = 'E') and
      (Accidental = AccNatural) then YMGetPeriod := N_E7;
    if (NoteName = 'E') and
      (Accidental = AccFlat) then YMGetPeriod := N_DS7;
    if (NoteName = 'F') and
      (Accidental = AccNatural) then YMGetPeriod := N_F7;
    if (NoteName = 'F') and
      (Accidental = AccSharp) then YMGetPeriod := N_FS7;
    if (NoteName = 'G') and
      (Accidental = AccNatural) then YMGetPeriod := N_G7;
    if (NoteName = 'G') and
      (Accidental = AccSharp) then YMGetPeriod := N_GS7;
    if (NoteName = 'G') and
      (Accidental = AccFlat) then YMGetPeriod := N_FS7;
    if (NoteName = 'A') and
      (Accidental = AccNatural) then YMGetPeriod := N_A7;
    if (NoteName = 'A') and
      (Accidental = AccSharp) then YMGetPeriod := N_AS7;
    if (NoteName = 'A') and
      (Accidental = AccFlat) then YMGetPeriod := N_GS7;
    if (NoteName = 'B') and
      (Accidental = AccNatural) then YMGetPeriod := N_B7;
    if (NoteName = 'B') and
      (Accidental = AccFlat) then YMGetPeriod := N_AS7;
  end;
end;

{ Play a named note for a given length }
procedure YMPlayNote(Channel : integer; NoteName, Accidental : char;
  Octave, Volume, Length : integer);
begin
  YMPlayPeriod(Channel,
  YMGetPeriod(NoteName, Accidental, Octave), Volume, Length);
end;

{ Play a named note using envelope volume }
procedure YMPlayEnvNote(Channel : integer; NoteName, Accidental : char;
  Octave, Length : integer);
begin
  YMPlayEnvPeriod(Channel,
  YMGetPeriod(NoteName, Accidental, Octave), Length);
end;

{ Play silence on one channel for a given length }
procedure YMRest(Channel, Length : integer);
begin
  YMNoteOff(Channel);
  YMDelayUnits(Length);
end;

{ Helpers for assigning musical roles to the three chip channels }
procedure YMPlayLead(NoteName, Accidental : char;
  Octave, Volume, Length : integer);
begin
  YMPlayNote(YMLeadChan, NoteName, Accidental, Octave, Volume, Length);
end;

procedure YMPlayHarmony(NoteName, Accidental : char;
  Octave, Volume, Length : integer);
begin
  YMPlayNote(YMHarmonyChan, NoteName, Accidental, Octave, Volume, Length);
end;

procedure YMPlayBass(NoteName, Accidental : char;
  Octave, Volume, Length : integer);
begin
  YMPlayNote(YMBassChan, NoteName, Accidental, Octave, Volume, Length);
end;

{ Play selected major triads using all three channels }
procedure YMPlayMajorTriad(RootName, RootAcc : char;
  Octave, Volume, Length : integer);
var
P1, P2, P3 : integer;

begin
  P1 := 0;
  P2 := 0;
  P3 := 0;
  
  if (RootName = 'C') and (RootAcc = AccNatural) then
    begin
      P1 := YMGetPeriod('C', AccNatural, Octave);
      P2 := YMGetPeriod('E', AccNatural, Octave);
      P3 := YMGetPeriod('G', AccNatural, Octave);
    end;

  if (RootName = 'F') and (RootAcc = AccNatural) then
    begin
      P1 := YMGetPeriod('F', AccNatural, Octave);
      P2 := YMGetPeriod('A', AccNatural, Octave);
      P3 := YMGetPeriod('C', AccNatural, Octave + 1);
    end;

  if (RootName = 'G') and (RootAcc = AccNatural) then
    begin
      P1 := YMGetPeriod('G', AccNatural, Octave);
      P2 := YMGetPeriod('B', AccNatural, Octave);
      P3 := YMGetPeriod('D', AccNatural, Octave + 1);
    end;

  if P1 <> 0 then
    begin
      YMNoteOn(YMChanA, P1, Volume);
      YMNoteOn(YMChanB, P2, Volume);
      YMNoteOn(YMChanC, P3, Volume);
      YMDelayUnits(Length);
      YMAllOff;
    end;
end;

{ Play selected minor triads using all three channels }
procedure YMPlayMinorTriad(RootName, RootAcc : char;
  Octave, Volume, Length : integer);
var
  P1, P2, P3 : integer;

begin
  P1 := 0;
  P2 := 0;
  P3 := 0;

  if (RootName = 'A') and (RootAcc = AccNatural) then
    begin
      P1 := YMGetPeriod('A', AccNatural, Octave);
      P2 := YMGetPeriod('C', AccNatural, Octave + 1);
      P3 := YMGetPeriod('E', AccNatural, Octave + 1);
    end;

  if (RootName = 'D') and (RootAcc = AccNatural) then
    begin
      P1 := YMGetPeriod('D', AccNatural, Octave);
      P2 := YMGetPeriod('F', AccNatural, Octave);
      P3 := YMGetPeriod('A', AccNatural, Octave);
    end;

  if (RootName = 'E') and (RootAcc = AccNatural) then
    begin
      P1 := YMGetPeriod('E', AccNatural, Octave);
      P2 := YMGetPeriod('G', AccNatural, Octave);
      P3 := YMGetPeriod('B', AccNatural, Octave);
    end;

  if P1 <> 0 then
    begin
      YMNoteOn(YMChanA, P1, Volume);
      YMNoteOn(YMChanB, P2, Volume);
      YMNoteOn(YMChanC, P3, Volume);
      YMDelayUnits(Length);
      YMAllOff;
    end;
end;
