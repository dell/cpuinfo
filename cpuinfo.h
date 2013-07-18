#ifndef __cpuinfo_h__
#define __cpuinfo_h__

#if 0
5-4-3 "02 P1m Pentium-MMX (P55C)
6-1-9 "00 P1m Pentium-Pro ()
6-5-2 "02 P2 Pentium-II (Deschutes)*
6-6-5 "00 P2 Celeron (Mendocino)
6-8-6 "02 P3 Pentium-III (Coppermine)
6-8-A "01 P3 Celeron (Coppermine)
6-9-5 "16 PM Pentium M (Banias)
6-B-1 "2004 P3 Pentium-III (Tualatin)
6-D-8 "16 PM Pentium M (Dothan) {error:  PM Core Duo (Yonah) when L2=4Meg}
6-E-8 "220 PM Core Duo (Yonah)*
6-F-6 "220 C2D Core 2 Duo (Conroe)
6-F-6 "2220 C2Dx Xeon 51xx (Woodcrest)
F-2-5 "09 P4 Xeon (Gallatin) {P4 Xeon (Prestonia B) L1=512K, L2=2Meg)
F-2-5 "200B P4 Xeon (Prestonia B)
F-2-7 "09 P4 Pentium-IV (Northwood)
F-2-7 "0E P4 Pentium-IV (Northwood)* {error: PD Pentium D (Presler)}
F-2-7 "200B Xeon DP (Prestonia) {error: PDx Xeon D 70 (Paxville) }
F-2-9 "09 P4 Pentium-IV (Northwood)*
F-2-9 "0B P4 Xeon DP (Prestonia)
F-3-4 "00 P4 Celeron (Prescott)
F-3-4 "00 P4 Pentium-IV (Prescott)
F-3-4 "00 P4 Pentium-IV (Prescott)* {error: Xeon P4 (Nocona) when L2=2M}
F-4-1 "00 P4 Pentium-IV (Prescott)*
F-4-1 "00 P4 Celeron (Prescott) {error: Pentium-IV (Prescott) when L2 = 256K}
F-4-1 "2000 P4 Xeon MP (Cranford)*
F-4-3 "00 P4 Pentium-IV (Prescott)* {error: PD Pentium D (Smithfield) when L2=2M}
F-4-3 "2000 P4 Xeon DP (Irwindale)
F-4-4 "00 PD Pentium D (Smithfield)
F-4-7 "210 PD Pentium D (Smithfield)*
F-6-4 "210 PD Pentium D (Presler)*

AMD
5-7-0 "00 K6 (Little Foot)
5-8-C "00 K6! K6-II 3D (Chompers)
5-9-1 "00 K6! K6-III K6-3 (Sharptooth)
6-3-1 "00 K7 Duron (Spitfire)
6-4-4 "00 K7 Athlon (Thunderbird)*
6-6-2 "00 K7x Athlon XP (Palomino)
6-8-0 "00 K7x Athlon XP (Thoroughbred)
6-A-0 "00  K7x Athlon XP (Barton)*
F-3-2 "B04 K8 Opteron 1 DC (xxx) [Denmark]*
F-7-1 "304 K8 Opteron 1 (Sledgehammer) [Venus]*
F-1-2 "C05 K8 Opteron DC 2 (xxx)[Italy] *
F-4-A "106 K8 Athlon 64 (Clawhammer)
F-C-0 "108 K8 Athlon 64 (Newcastle)*
F-7-A "10D K8 Athlon 64 (Newcastle)*
F-F-0 "10D K8 Athlon 64 (Winchester)
F-7-1 "10F K8 Athlon 64 (San Diego)*
F-4-2 "20F K8 Athlon 64 (Newark)*
F-B-1 "150 K8 Athlon 64 X2 (Manchester)
F-3-2 "15A K8 Athlon 64 X2 (Toledo)*
F-C-2 "8C6 K8 Sempron (Albany)
F-2C-2 "982 K8 Sempron (Manila)*
F-24-2 "2CA K8 Turion 64 (Lancaster)*
#endif

struct modelinfo {
  int family, model, stepping;
  char *descr;
} cputable[] = {
  { 15, 0, 1, "Willamette ES" },
  { 15, 0, 5, "Willamette B2" },
  { 15, 0, 10,"Willamette C1" },
  { 15, 1, 2, "Willamette D0" },
  { 15, 1, 3, "Willamette E0" },
  { 15, 2, 4, "Northwood B0" },
  { 15, 2, 4, "Northwood B0" },
  { 15, 2, 4, "Northwood B0" },
  { 15, 2, 5, "Northwood M0" },
  { 15, 2, 7, "Northwood C1" },
  { 15, 2, 9, "Northwood D1" },
  { 15, 2, 5, "Gallatin" },
  { 15, 2, 5, "Gallatin" },
  { 
