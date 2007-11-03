crab -create -cfg datasets/crab_inclTTbar.cfg

~/bin/create_links_crab.py

crab -submit all -c crab_inclTTbar
