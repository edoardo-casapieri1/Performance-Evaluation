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
paths = config_parameters['paths_conf1']
dist = config_parameters['distribution']
# --------------------------------------------------------------------------#


# path = r'C:\Users\Riccardo\Downloads\results\results'
# path = r'C:\Users\Riccardo\Documents\GitHub\Performance_final_version\Configurazione1_2\Configurazione1\simulations\results'


for path in paths:
    os.chdir(path)
    # os.system("scavetool x General-5,1#2f2,1#2f20,1#2f30,0.6-*.vec -o General-5,1#2f2,1#2f20,1#2f30,0.6.csv")
    for a in range(len(initialInterArrivalRate)):
        for b in range(len(checkoutRate)):
            for c in range(len(wrappingRate)):
                for d in range(len(wrappingProbability)):
                    src = "General-n=" + numb_of_tills + ',' + initialInterArrivalRate[a] + ',' + '1#2f' + \
                          checkoutRate[b] + ',' + '1#2f' + wrappingRate[c] + ',' + wrappingProbability[
                              d] + ","+ dist + '-*.vec'
                    dest = "General-n=" + numb_of_tills + ',' + initialInterArrivalRate[a] + ',' + '1#2f' + \
                           checkoutRate[b] + ',' + '1#2f' + wrappingRate[c] + ',' + wrappingProbability[d]
                    cmd = "scavetool x " + src + " -o " + dest + '.csv'
                    os.system(cmd)
