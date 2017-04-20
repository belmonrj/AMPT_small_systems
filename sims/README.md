Simulation log
==============


** dAu 200 GeV **

- set 0: 200 GeV, b<2 fm, sig_parton=0.75 mb, FVTX pT filter, 5k events/job, 10k jobs (50M total)
- set 1: 200 GeV, b<2 fm, sig_parton=0.75 mb, FVTX 1<|eta|<3 only, 5k events/job, 10k jobs (50M total)
- set 2: 200 GeV, b<2 fm, sig_parton=1.50 mb, FVTX 1<|eta|<3 only, 5k events/job, 10k jobs (50M total)
- set 3: 200 GeV, b<2 fm, sig_parton=1.50 mb, FVTX pT filter, 5k events/job, 10k jobs (50M total)
- set 4: 200 GeV, b<20 fm, sig_parton=1.50 mb, FVTX pT filter, 5k events/job, 10k jobs (50M total)
- set 5: 200 GeV, b<20 fm, sig_parton=1.50 mb, FVTX pT filter, 5k events/job, 10k jobs (50M total)
    + Changed binning on N_charge (500, -0.5, 499.5)
    + Add 2D for <<4>>
    + Add parton_pplane.C
- set 6: 200 GeV, b<2 fm, sig_parton=1.50 mb, FVTX pT filter, 5k events/job, 1k jobs (5M total)
    + Modified parton_pplane.C to use particles in FVTX range for Nch dependence


** pAu 200 GeV **

- set 0: 200 GeV, b<2 fm, sig_parton=1.50 mb, FVTX pT filter, 5k events/job, 10k jobs (50M total)


** AuAu 200 GeV **

- set 0: 200 GeV, b<20 fm, sig_parton=1.50 mb, FVTX pT filter, 1k events/job, 1k jobs (1M total)
