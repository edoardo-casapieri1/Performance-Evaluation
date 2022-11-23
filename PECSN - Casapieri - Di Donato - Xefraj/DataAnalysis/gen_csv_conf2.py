import os
import json

# ------------------ ONLY PARAMETER TO CHANGE IN THIS CODE ----------------#
with open('config.json') as f:
    config_parameters = json.load(f)

numb_of_tills = config_parameters['numb_of_tills']
initialInterArrivalRate = config_parameters['initialInterArrivalRate']
checkoutRate = config_parameters['checkoutRate']
wrappingRate = config_parameters['wrappingRate']
wrappingProbability = config_parameters['wrappingProbability']
paths = config_parameters['paths_conf2']
NCheck = [numb_of_tills + '#2f3', '(' + numb_of_tills + ')' + '#2a2#2f3', numb_of_tills + '#2f2']
dist = config_parameters['distribution']
# --------------------------------------------------------------------------#





# os.system("scavetool x General-5,1#2f2,1#2f20,1#2f30,0.6-*.vec -o General-5,1#2f2,1#2f20,1#2f30,0.6.csv")
for path in paths:
    os.chdir(path)
    for s in range(len(NCheck)):
        #NWrap = str(int(numb_of_tills) - int(NCheck[s]))
        for a in range(len(initialInterArrivalRate)):
            for b in range(len(checkoutRate)):
                for c in range(len(wrappingRate)):
                    for d in range(len(wrappingProbability)):
                        src = "General-n=" + numb_of_tills + ','+ 'nc=' +  NCheck[s]+ ',' + initialInterArrivalRate[a] + ',' + '1#2f' + \
                          checkoutRate[b] + ',' + '1#2f' + wrappingRate[c] + ',' + wrappingProbability[d] +','+ dist + '-*.vec'
                        dest = "General-n=" + numb_of_tills + ','+ 'nc=' +  NCheck[s]+ ',' + initialInterArrivalRate[a] + ',' + '1#2f' + \
                          checkoutRate[b] + ',' + '1#2f' + wrappingRate[c] + ',' + wrappingProbability[d]
                        cmd = "scavetool x " + src + " -o " + dest + '.csv'
                        os.system(cmd)