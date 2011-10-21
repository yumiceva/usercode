#!/bin/csh
#

if( $argv[1] == "muons" ) then
  echo "Setup files to run on MUONS"
  rm -f Wp_exp_limit.cfg Wp_obs_limits.cfg Wp_templates.cfg commonWp.cfg
  ln -s Wp_exp_limit_mu.cfg Wp_exp_limit.cfg
  ln -s Wp_obs_limits_mu.cfg Wp_obs_limits.cfg
  ln -s commonWp_mu.cfg commonWp.cfg
  ln -s Wp_templates_mu.cfg Wp_templates.cfg
else if( $argv[1] == "electrons" ) then
  echo "Setup files to run on ELECTRONS"
  rm -f Wp_exp_limit.cfg Wp_obs_limits.cfg Wp_templates.cfg commonWp.cfg
  ln -s Wp_exp_limit_e.cfg Wp_exp_limit.cfg
  ln -s Wp_obs_limits_e.cfg Wp_obs_limits.cfg
  ln -s commonWp_e.cfg commonWp.cfg
  ln -s Wp_templates_e.cfg Wp_templates.cfg
else if( $argv[1] == "combined" ) then
  echo "Setup files to run COMBINED"
  rm -f Wp_exp_limit.cfg Wp_obs_limits.cfg Wp_templates.cfg commonWp.cfg
  ln -s Wp_exp_limit_emu.cfg Wp_exp_limit.cfg
  ln -s Wp_obs_limits_emu.cfg Wp_obs_limits.cfg
  ln -s commonWp_emu.cfg commonWp.cfg
  ln -s Wp_templates_emu.cfg Wp_templates.cfg
endif
