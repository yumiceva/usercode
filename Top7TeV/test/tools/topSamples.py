

def Samples( top, dataType, dataTypeSuffix ):

    # prefix location
    data_repo = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/"
    #datafilename = "Data/21.89pb-1/ttmuj_21.89pb-1_Oct29.root"
    #datafilename = "/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Nov3/TrigC/TrigC.root"

    if dataType=="data" or dataType=="dataReverse":
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/Run2010A_Mu9.root")
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/Run2010B_Mu9.root")
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/Run2010B_Mu15.root")
    if dataType=="TTbar_Z2":
        top.Add(data_repo+"MC/V00-01-05/TTbar_Z2_Mu.root")
    if dataType=="TTbar_D6T":
        top.Add(data_repo+"MC/V00-01-05/TTbar_D6T_Mu.root")
    if dataType=="sync":
        top.Add("/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Oct5/TTJets_syncv4.root")
    if dataType=="Wjets_Z2":
        top.Add(data_repo+"MC/V00-01-06/WJets_Z2_Mu.root")
    if dataType=="Wjets_D6T":
        top.Add(data_repo+"MC/V00-01-06/WJets_D6T_Mu.root")
    if dataType=="WW_Z2":
        top.Add(data_repo+"MC/V00-01-05/WW_Z2_Mu.root")
    if dataType=="Zjets_Z2":
        top.Add(data_repo+"MC/V00-01-06/ZJets_Z2_Mu.root")
    if dataType=="Zjets_D6T":
        top.Add(data_repo+"MC/V00-01-06/ZJets_D6T_Mu.root")
    if dataType=="QCD":
        top.Add(data_repo+"MC/V00-01-05/QCD_Z2_Mu.root")
    if dataType=="STtch":
        top.Add(data_repo+"MC/V00-01-05/STtch_Z2_Mu.root")
    if dataType=="STtWch":
        top.Add(data_repo+"MC/V00-01-05/STtWch_Z2_Mu.root")
    if dataType=="STsch":
        top.Add(data_repo+"MC/V00-01-05/STsch_Z2_Mu.root")
    if dataType=="Wc":
        top.Add(data_repo+"MC/V00-01-05/Wc_D6T_Mu.root")
    if dataType=="Vqq":
        top.Add(data_repo+"MC/V00-01-06/Vqq_D6T_Mu.root")
    if dataType=="TTbar_matchingup":
        dataType="TTbar"
        dataTypeSuffix = "matchingup"
        top.Add(data_repo+"MC/V00-01-05/TTbar_matchingup_Mu.root")
    if dataType=="TTbar_matchingdown":
        dataType="TTbar"
        dataTypeSuffix = "matchingdown"
        top.Add(data_repo+"MC/V00-01-05/TTbar_matchingdown_Mu.root")
    if dataType=="TTbar_scaleup":
        dataType="TTbar"
        dataTypeSuffix = "scaleup"
        top.Add(data_repo+"MC/V00-01-05/TTbar_scaleup_Mu.root")
    if dataType=="TTbar_scaledown":
        dataType="TTbar"
        dataTypeSuffix = "scaledown"
        top.Add(data_repo+"MC/V00-01-05/TTbar_scaledown_Mu.root")
    if dataType=="Wjets_matchingup":
        dataType="Wjets"
        dataTypeSuffix = "matchingup"
        top.Add(data_repo+"MC/V00-01-05/WJets_matchingup_Mu.root")
    if dataType=="Wjets_matchingdown":
        dataType="Wjets"
        dataTypeSuffix = "matchingdown"
        top.Add(data_repo+"MC/V00-01-05/WJets_matchingdown_Mu.root")
    if dataType=="Wjets_scaleup":
        dataType="Wjets"
        dataTypeSuffix = "scaleup"
        top.Add(data_repo+"MC/V00-01-05/WJets_scaleup_Mu.root")
    if dataType=="Wjets_scaledown":
        dataType="Wjets"
        dataTypeSuffix = "scaledown"
        top.Add(data_repo+"MC/V00-01-05/WJets_scaledown_Mu.root")
    if dataType=="Zjets_matchingup":
        dataType="Zjets"
        dataTypeSuffix = "matchingup"
        top.Add(data_repo+"MC/V00-01-05/ZJets_matchingup_Mu.root")
    if dataType=="Zjets_matchingdown":
        dataType="Zjets"
        dataTypeSuffix = "matchingdown"
        top.Add(data_repo+"MC/V00-01-05/ZJets_matchingdown_Mu.root")
    if dataType=="Zjets_scaleup":
        dataType="Zjets"
        dataTypeSuffix = "scaleup"
        top.Add(data_repo+"MC/V00-01-05/ZJets_scaleup_Mu.root")
    if dataType=="Zjets_scaledown":
        dataType="Zjets"
        dataTypeSuffix = "scaledown"
        top.Add(data_repo+"MC/V00-01-05/ZJets_scaledown_Mu.root")
    if dataType=="Wprime_1TeV":
        dataType="Wprime_1TeV"
        top.Add("/uscms_data/d2/marafino/Wprime_tbbar-Ntuple-PATskim.root")
    if dataType=="Wprime_1p2TeV":
        dataType="Wprime_1p2TeV"
        top.Add("/uscms_data/d2/perera//WprimeNT/WprimetB-Ntuple-PATskim_1200.root")
    if dataType=="TPrime_400M":
        top.Add("/uscms_data/d3/ttmuj/Documents/NtupleMaker/MC/V00-01-07/TPrime_400M_Mu-Ntuple-PATskim.root")
    if dataType=="HToWW_M300":
        top.Add("/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Mar3/HToWW_M300_Mu/HToWW_M300_Mu.root")
        
    return (dataType, dataTypeSuffix)

def eleSamples( top, dataType, dataTypeSuffix ):

    # prefix location
    data_repo = "/uscms_data/d3/ttmuj/Documents/NtupleMaker/"
    
    if dataType=="data":
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/electronsV3/TrigA.root")
        #top.Add("/uscmst1b_scratch/lpc1/cmsroc/yumiceva/top_prod_Mar7/TrigA/TrigA.root")
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/electronsV3/TrigB.root")
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/electronsV3/TrigC.root")
        #top.Add(data_repo+"Data/36pb-1_Nov4ReReco/electronsV3/PromptRecoTrigC.root")
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/electronsV3/TrigD.root")
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/electronsV3/TrigE.root")
        top.Add(data_repo+"Data/36pb-1_Nov4ReReco/electronsV3/TrigF.root")

    else:
        return Samples( top, dataType, dataTypeSuffix)

    return (dataType, dataTypeSuffix)

