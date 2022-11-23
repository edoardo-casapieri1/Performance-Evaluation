import pandas as pd
import numpy as np
import json
# ------------------ ONLY PARAMETER TO CHANGE IN THIS CODE ----------------#
user_index = 0 # 0:Riccardo 1:Edoardo


with open('config.json') as f:
    config_parameters = json.load(f)

# This is new factor for policy 2
NCheck = ['(6)#2a2#2f3', '6#2f3']

tills = config_parameters['numb_of_tills']

initialInterArrivalRates = config_parameters['initialInterArrivalRate']
checkoutRates = config_parameters['checkoutRate']
wrappingRates = config_parameters['wrappingRate']
wrappingProbabilities = config_parameters['wrappingProbability']
paths = config_parameters['paths_conf2']

initialInterArrivalRate = [initialInterArrivalRates[0]] + [initialInterArrivalRates[-1]]
checkoutRate = [checkoutRates[0]] + [checkoutRates[-1]]
wrappingRate = [wrappingRates[0]] + [wrappingRates[-1]]
wrappingProbability = [wrappingProbabilities[0]] + [wrappingProbabilities[-1]]

print(initialInterArrivalRate)
# --------------------------------------------------------------------------#

def parse_if_number(s):
    try:
        return float(s)
    except:
        return True if s == "true" else False if s == "false" else s if s else None


def parse_ndarray(s):
    return np.fromstring(s, sep=' ') if s else None


for path in paths:
    f = open(path+"/results/2kr-conf2.txt", "w")

    for d in range(len(wrappingProbability)):
        for c in range(len(wrappingRate)):
            for b in range(len(checkoutRate)):
                for a in range(len(initialInterArrivalRate)):
                    for s in range(len(NCheck)):
                        filename = "General-n="+ tills + ','+ 'nc=' +  NCheck[s]+ ',' + initialInterArrivalRate[a] + ',' + '1#2f' + \
                        checkoutRate[b] + ',' + '1#2f' + wrappingRate[c] + ',' + wrappingProbability[d] + '.csv'

                        print("Conf1: 2kr OPENING FILE: " + filename + " Index: " +str(s)+ str(a) + str(b) + str(c) + str(d) )

                        file = pd.read_csv(
                            path + "/" +
                            filename,
                            converters={'attrvalue': parse_if_number, 'binedges': parse_ndarray, 'binvalues': parse_ndarray,
                                  'vectime': parse_ndarray, 'vecvalue': parse_ndarray}
                        )
                        tmp = file[(file.type == 'vector') & ((file.name == "signalResponseTimeCheckOut:vector") | (file.name =="signalResponseTimeCheckOutAndWrap:vector"))]
                        #tmp = file[(file.type == 'vector') & (file.name =="signalResponseTimeCheckOutAndWrap:vector")]
                        seed_name = []
                        index_seed_array = 0
                        for row in tmp.itertuples():
                            if row.run not in seed_name:
                                seed_name.insert(index_seed_array, row.run)
                                index_seed_array = index_seed_array + 1
                        TotalValuesSubsampling = 0
                        TotalNumberElements = 0
                        for i in range(len(seed_name)):
                            current_seed = seed_name[i]
                            current_tmp = tmp[(tmp.run == current_seed)]
                            for row in current_tmp.itertuples():
                                row_1_vecValue = pd.Series(row.vecvalue)
                                count = int(np.round(row_1_vecValue.count() / 10))
                                #print(count)
                                row_2_sub = row_1_vecValue.loc[np.random.choice(row_1_vecValue.index, count, replace=False)]
                                for sub in row_2_sub:
                                    TotalValuesSubsampling = TotalValuesSubsampling + sub
                                TotalNumberElements = TotalNumberElements + len(row_2_sub)
                            seedAverage = TotalValuesSubsampling / TotalNumberElements
                            if i == 9:
                                f.write(str(seedAverage))
                            else:
                                f.write(str(seedAverage) + " ")      #aggiungo una colonna
                            TotalValuesSubsampling = 0
                            TotalNumberElements = 0
                        f.write("\n")
    f.close()