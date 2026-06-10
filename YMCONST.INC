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
