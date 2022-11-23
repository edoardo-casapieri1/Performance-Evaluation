import numpy as np
import matplotlib.pyplot as plt
import os
import pandas as pd
import json

# ------------------ ONLY PARAMETER TO CHANGE IN THIS CODE ----------------#
user_index = 0  # 0:Riccardo 1:Edoardo

with open('config.json') as f:
    config_parameters = json.load(f)

# Aggregate booth arrays
paths = config_parameters['paths_conf1'] + config_parameters['paths_conf2']
numb_of_tills = config_parameters['numb_of_tills']

initialInterArrivalRates = config_parameters['initialInterArrivalRate']
checkoutRates = config_parameters['checkoutRate']
wrappingRates = config_parameters['wrappingRate']
wrappingProbabilities = config_parameters['wrappingProbability']

initialInterArrivalRate = initialInterArrivalRates[0]
checkoutRate = checkoutRates[-1]
wrappingRate = wrappingRates[-1]
wrappingProbability = wrappingProbabilities[-1]

worst_case_scenario_title = initialInterArrivalRate + ",1#2f" + checkoutRate + ",1#2f" + wrappingRate + "," + wrappingProbability + ".csv"


# --------------------------------------------------------------------------#
def gini(arr):
    count = arr.size
    coefficient = 2 / count
    indexes = np.arange(1, count + 1)
    weighted_sum = (indexes * arr).sum()
    total = arr.sum()
    constant = (count + 1) / count
    return coefficient * weighted_sum / total - constant


def lorenz(arr):
    # this divides the prefix sum by the total sum
    # this ensures all the values are between 0 and 1.0
    scaled_prefix_sum = arr.cumsum() / arr.sum()
    # this prepends the 0 value (because 0% of all people have 0% of all wealth)
    return np.insert(scaled_prefix_sum, 0, 0)


def rchop(s, suffix):
    if suffix and s.endswith(suffix):
        return s[:-len(suffix)]
    return s


def parse_if_number(s):
    try:
        return float(s)
    except:
        return True if s == "true" else False if s == "false" else s if s else None


def parse_ndarray(s):
    return np.fromstring(s, sep=' ') if s else None


for path in paths:
    os.chdir(path)
    # os.system("scavetool x General-5,1#2f2,1#2f20,1#2f30,0.6-*.vec -o General-5,1#2f2,1#2f20,1#2f30,0.6.csv")

    files = []
    # Take only the files for the worst case
    for root, dirs, files_in_dir in os.walk(path):
        for file in files_in_dir:
            if worst_case_scenario_title in file:
                files.append(file)

    print(files)
    signals = ["signalResponseTimeCheckOut", "signalResponseTimeCheckOutAndWrap"]
    # for each file get with worst parameters:
    # Policy1: Only a worst case scenario
    # Policy2: A worst case scenario for 2/3; 1/3; 1/2
    for file_worst_case in files:
        for y in range(len(signals)):
            samples_all_seed = []
            # delete .csv from file name
            # new_filename = rchop(file_worst_case, '.csv')
            # INFORMATION ABOUT FILE
            # f = open(ResultPath + new_filename + "_"+signals[y]+".txt", "a")
            # f.close()
            print("Reading data from: " + file_worst_case)
            file = pd.read_csv(
                path + "/" +
                file_worst_case,
                converters={'attrvalue': parse_if_number, 'binedges': parse_ndarray, 'binvalues': parse_ndarray,
                            'vectime': parse_ndarray, 'vecvalue': parse_ndarray})
            if y < 4:
                tmp = file[(file.type == 'vector') & (file.name == signals[y] + ":vector")]
            else:
                tmp = file[(file.type == 'vector') & (
                            (file.name == signals[0] + ":vector") | (file.name == signals[1] + ":vector"))]
            seed_name = []
            index_seed_array = 0

            # number of seeds of the scenario
            for row in tmp.itertuples():
                if row.run not in seed_name:
                    seed_name.insert(index_seed_array, row.run)
                    index_seed_array = index_seed_array + 1  # da correggere anche gli altri file py

            # subsample for each seed
            for i in range(len(seed_name)):
                current_seed = seed_name[i]
                current_tmp = tmp[(tmp.run == current_seed)]
                sample_seed = pd.Series([], dtype='float64')
                # count_sample_seed = 0

                # subsample customer time in each till
                for row in current_tmp.itertuples():
                    # row_1_vecTime = pd.Series(row.vectime)
                    row_1_vecValue = pd.Series(row.vecvalue)
                    # row_2 = pd.Series([], dtype='float64')
                    # count_warmup = 0
                    # for index, value in row_1_vecTime.items():
                    #    if value > 100000:
                    #        app = row_1_vecValue[index]
                    #        row_2 = row_2.append(pd.Series([app], index=[count_warmup]))
                    #        count_warmup = count_warmup + 1
                    # lag_plot(row_2, lag=1)
                    # plt.title(row.module + " Total" + seed_name[i])
                    # plt.show()

                    # SUBSAMPLING
                    count = int(np.round(row_1_vecValue.count() / 10))  # DA CONTROLLARE
                    row_2_sub = row_1_vecValue.loc[np.random.choice(row_1_vecValue.index, count, replace=False)]
                    sample_seed = sample_seed.append(row_2_sub)
                    # print(sample_seed)
                    # lag_plot(row_2_sub, lag=1)
                    # plt.title(row.module + " Sub" + seed_name[i])
                    # plt.show()
                    # count_sample_seed = count_sample_seed + 1
                # mean_app = np.mean(sample_seed)
                for g in range(len(sample_seed.values)):
                    samples_all_seed.append(sample_seed.values[g])

            # ensure your arr is sorted from lowest to highest values first!
            arr = np.sort(np.array(samples_all_seed))
            #print(arr)
            # np.array([6,6,6,7,7,8])
            # show the gini index!
            # print(gini(arr))

            lorenz_curve = lorenz(arr)

            # we need the X values to be between 0.0 to 1.0
            plt.plot(np.linspace(0.0, 1.0, lorenz_curve.size), lorenz_curve)
            # plot the straight line perfect equality curve
            plt.plot([0, 1], [0, 1])
            plt.title("signal = " + signals[y] + ', GINI = ' + str(gini(arr))[0:5])
            plt.savefig(path + "/results/LC_"+file_worst_case[0:-4]+"_" + signals[y] + ".pdf")
            plt.close()
            print("FINISH: " + file_worst_case)
