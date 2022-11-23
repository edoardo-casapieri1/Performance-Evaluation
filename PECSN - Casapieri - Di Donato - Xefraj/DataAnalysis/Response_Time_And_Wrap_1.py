import pandas as pd
import numpy as np
import os
import json
import re
import matplotlib.pyplot as plt
from pandas import DataFrame
from pandas.plotting import lag_plot

# ------------------ ONLY PARAMETER TO CHANGE IN THIS CODE ----------------#

with open('config.json') as f:
    config_parameters = json.load(f)

paths = config_parameters['paths_conf1']


# --------------------------------------------------------------------------#


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
    for root, dirs, files_in_dir in os.walk(path):
        for file in files_in_dir:
            if '.csv' in file:
                files.append(file)

    signals = ["signalResponseTimeCheckOutAndWrap", "signalResponseTimeCheckOut", "signalWaitingTimeCheckOutAndWrap",
               "signalWaitingTimeCheckOut", "Cumulative_response_time"]
    for y in range(len(signals)):
        for k in range(len(files)):

            new_filename = rchop(files[k], '.csv')
            # INFORMATION ABOUT FILE
            # f = open(ResultPath + new_filename + "_"+signals[y]+".txt", "a")
            # f.close()
            print(files[k])
            file = pd.read_csv(
                path + "/" +
                files[k],
                converters={'attrvalue': parse_if_number, 'binedges': parse_ndarray, 'binvalues': parse_ndarray,
                            'vectime': parse_ndarray, 'vecvalue': parse_ndarray})
            # Take care of cumulative signal
            if y < 4:
                tmp = file[(file.type == 'vector') & (file.name == signals[y] + ":vector")]
            else:
                tmp = file[(file.type == 'vector') & (
                            (file.name == signals[0] + ":vector") | (file.name == signals[1] + ":vector"))]
            seed_name = []
            index_seed_array = 0
            # check how many seeds in total
            for row in tmp.itertuples():
                if row.run not in seed_name:
                    seed_name.insert(index_seed_array, row.run)
                    index_seed_array = index_seed_array + 1  # da correggere anche gli altri file py

            samples_all_seed = []
            index_samples_all_seed = 0

            # for each seed take a sub-sample
            for i in range(len(seed_name)):
                current_seed = seed_name[i]
                current_tmp = tmp[(tmp.run == current_seed)]
                sample_seed = pd.Series([], dtype='float64')
                # count_sample_seed = 0
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
                    # lag_plot(row_2_sub, lag=1)
                    # plt.title(row.module + " Sub" + seed_name[i])
                    # plt.show()
                    # count_sample_seed = count_sample_seed + 1
                # mean_app = np.mean(sample_seed)
                for g in range(len(sample_seed.values)):
                    samples_all_seed.append(sample_seed.values[g])
                index_samples_all_seed = index_samples_all_seed + len(sample_seed) + 1

            x_bar = np.mean(samples_all_seed)
            n = len(samples_all_seed)
            z_alpha = 1.960

            summation = 0
            for j in range(len(samples_all_seed)):
                app_sum = samples_all_seed[j] - x_bar
                summation = summation + (app_sum * app_sum)

            S_square = summation / (n - 1)

            start = x_bar - ((np.sqrt(S_square) / np.sqrt(n)) * z_alpha)
            end = x_bar + ((np.sqrt(S_square) / np.sqrt(n)) * z_alpha)

            f = open(path + "/" + new_filename + "_" + signals[y] + ".txt", "w")
            # f.write("Segnale: signalResponseTimeCheckOutAndWrap:vector : \n")
            f.write("S_square: " + str(S_square) + "\n")
            f.write("x_bar: " + str(x_bar) + "\n")
            f.write("start: " + str(start) + "\n")
            f.write("end: " + str(end) + "\n")
            f.close()
