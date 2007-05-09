
cmsRun TestPythia.cfg > & qcd_gauss2.log
mv VtxTest.root VtxTest_qcd_gauss.root

cmsRun TestPythia.cfg > & qcd_beta_nocrossing2.log
mv VtxTest.root VtxTest_qcd_beta_nocrossing.root

cmsRun TestPythia.cfg > & qcd_beta_crossing2.log
mv VtxTest.root VtxTest_qcd_beta_crossing.root

cmsRun TestPythia.cfg > & qcd_beta_crossing_with_cms2.log
mv VtxTest.root VtxTest_qcd_beta_crossing_with_cms.root
