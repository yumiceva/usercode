#!/bin/csh
#

# expected
root -l -q $argv[1]_800/results_expected_800.root median.C\(0,800\)
root -l -q $argv[1]_900/results_expected_900.root median.C\(0,900\)
root -l -q $argv[1]_1000/results_expected_1000.root median.C\(0,1000\)
root -l -q $argv[1]_1100/results_expected_1100.root median.C\(0,1100\)
root -l -q $argv[1]_1200/results_expected_1200.root median.C\(0,1200\)
root -l -q $argv[1]_1300/results_expected_1300.root median.C\(0,1300\)
root -l -q $argv[1]_1400/results_expected_1400.root median.C\(0,1400\)
root -l -q $argv[1]_1500/results_expected_1500.root median.C\(0,1500\)
root -l -q $argv[1]_1600/results_expected_1600.root median.C\(0,1600\)
root -l -q $argv[1]_2000/results_expected_2000.root median.C\(0,2000\)

# observed
root -l -q $argv[1]_800/results_observed_800.root median.C\(1,800\)
root -l -q $argv[1]_900/results_observed_900.root median.C\(1,900\)
root -l -q $argv[1]_1000/results_observed_1000.root median.C\(1,1000\)
root -l -q $argv[1]_1100/results_observed_1100.root median.C\(1,1100\)
root -l -q $argv[1]_1200/results_observed_1200.root median.C\(1,1200\)
root -l -q $argv[1]_1300/results_observed_1300.root median.C\(1,1300\)
root -l -q $argv[1]_1400/results_observed_1400.root median.C\(1,1400\)
root -l -q $argv[1]_1500/results_observed_1500.root median.C\(1,1500\)
root -l -q $argv[1]_1600/results_observed_1600.root median.C\(1,1600\)
root -l -q $argv[1]_2000/results_observed_2000.root median.C\(1,2000\)

