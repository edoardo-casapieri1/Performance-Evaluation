import numpy as np
import matplotlib.pyplot as plt
import json
import sys

# **********************************************************************************************************************

# Il seguente script presi gli input mette a confronto in un unico grafico Il il tempo di checkout Response/waiting con
# il tempo di wrap response/waiting. Questo viene fatto automaticamente per tutti i possibili numeri di casse di checkout

# **********************************************************************************************************************

# ------------------ ONLY PARAMETER TO CHANGE IN THIS CODE ----------------#
with open('config.json') as f:
    config_parameters = json.load(f)

path_result1s = config_parameters['paths_conf1']
path_result2s = config_parameters['paths_conf2']

numb_of_tills = config_parameters['numb_of_tills']

initialInterArrivalRates = config_parameters['initialInterArrivalRate']
checkoutRates = config_parameters['checkoutRate']
wrappingRates = config_parameters['wrappingRate']
wrappingProbabilities = config_parameters['wrappingProbability']

# check the parameter to change from command line
changing_parameter = int(sys.argv[1])

# take the worst value of all the other parameters
if changing_parameter == 0:
    print("selected changing InterArrival")
    Info = "InterArrival"
    x_values = initialInterArrivalRates
    print(checkoutRates)
    checkoutRates = ['' + checkoutRates[-1] + '']
    wrappingRates = ['' + wrappingRates[-1] + '']
    wrappingProbabilities = ['' + wrappingProbabilities[-1] + '']
    print(checkoutRates)
elif changing_parameter == 1:
    print("selected changing checkout rate")
    print(wrappingRates)
    Info = "CheckoutRate"
    x_values = checkoutRates
    initialInterArrivalRates = ['' + initialInterArrivalRates[0] + '']
    wrappingRates = ['' + wrappingRates[-1] + '']
    wrappingProbabilities = ['' + wrappingProbabilities[-1] + '']
    print(wrappingRates)
elif changing_parameter == 2:
    print("selected changing wrapping rate")
    x_values = wrappingRates
    Info = "WrappingRate"
    initialInterArrivalRates = ['' + initialInterArrivalRates[0] + '']
    checkoutRates = ['' + checkoutRates[-1] + '']
    wrappingProbabilities = ['' + wrappingProbabilities[-1] + '']
else:
    print("selected changing wrapping probability")
    x_values = wrappingProbabilities
    Info = "WrappingProbability"
    initialInterArrivalRates = ['' + initialInterArrivalRates[0] + '']
    checkoutRates = ['' + checkoutRates[-1] + '']
    wrappingRates = ['' + wrappingRates[-1] + '']

path_result1 = path_result1s[0]
path_result2 = path_result2s[0]
# --------------------------------------------------------------------------#


NCheck_array = ["1", numb_of_tills + '#2f2', numb_of_tills + '#2f3', '(' + numb_of_tills + ')' + '#2a2#2f3']
NCheck_array_label = ["NC:" + numb_of_tills, "NC:" + str(int(int(numb_of_tills) / 2)),
                      "NC:" + str(int(int(numb_of_tills) / 3)), "NC:" + str(int(int(numb_of_tills) * 2 / 3))]

outer = ["FULL-PLOT-_Cumulative_response_time", "FULL-PLOT-_Response_Time_CHECKOUT",
         "FULL-PLOT-_Response_Time_CHECKANDWRAp"]
signal1 = ["_Cumulative_response_time", "_signalResponseTimeCheckOut", "_signalResponseTimeCheckOutAndWrap"]

counter = 0

for i in range(len(signal1)):
    plt.figure(counter)
    # This will contain an array for each scenario 6 - 6/2 - 6/3 - 6*2/3 with inside
    # 5 values: the mean and error of each possible wrappingProbability for that specific scenario
    # x_bar_vec_tot = M*N (M=Number of scenarios N=Number of prob or varing signal considered for each scenario)
    x_bar_vec_tot = np.array([])
    start_vec_tot = np.array([])
    end_vec_tot = np.array([])
    e_vec_tot = np.array([])

    # Iterate through the number of tills
    for nCheck in NCheck_array:
        x_bar_vec = []
        start_vec = []
        end_vec = []
        e_vec = []

        # For a specified number of till consider a single probability at a time (0.6 then 0.65 ecc...)
        # And take this mean and error
        # out of for: An array of N values with N=probability considered
        # Only one between the factor has more than on value in its array
        for initialInterArrivalRate in initialInterArrivalRates:
            for checkoutRate in checkoutRates:
                for wrappingRate in wrappingRates:
                    for prob in wrappingProbabilities:

                        # if initialInterArrivalRate != "1":
                        #    initialInterArrivalRate = "1#2f" + initialInterArrivalRate
                        # In order to discriminate the scenario 1 from the 2
                        if nCheck == "1":
                            fileConf1 = open(
                                path_result1 + "/" + "General-n=" + numb_of_tills + ',' + initialInterArrivalRate + ',' + '1#2f' + checkoutRate + ',' + '1#2f' + wrappingRate + ',' + prob + '' +
                                signal1[i] + ".txt",
                                'r')
                            LinesConf = fileConf1.readlines()
                        else:
                            fileConf2_1 = open(
                                path_result2 + "/" + "General-n=" + numb_of_tills + ',' + "nc=" + nCheck + ',' + initialInterArrivalRate + ',' + '1#2f' + checkoutRate + ',' + '1#2f' + wrappingRate + ',' + prob + '' +
                                signal1[i] + ".txt",
                                'r')
                            LinesConf = fileConf2_1.readlines()

                        x_bar_vec.append(float(LinesConf[1][7:-1]))
                        start_vec.append(float(LinesConf[2][7:-1]))
                        end_vec.append(float(LinesConf[3][5:-1]))
                        e_vec.append(float(LinesConf[3][5:-1]) - float(LinesConf[2][7:-1]))
                        print("E_VECCC")
                        print(e_vec)

        if nCheck == "1":
            x_bar_vec_tot = np.append(x_bar_vec_tot, x_bar_vec)
            start_vec_tot = np.append(start_vec_tot, start_vec)
            end_vec_tot = np.append(end_vec_tot, end_vec)
            e_vec_tot = np.append(e_vec_tot, e_vec)
        else:
            x_bar_vec_tot = np.vstack((x_bar_vec_tot, x_bar_vec))
            start_vec_tot = np.vstack((start_vec_tot, start_vec))
            end_vec_tot = np.vstack((end_vec_tot, end_vec))
            e_vec_tot = np.vstack((e_vec_tot, e_vec))

    # Now x_bar_vec_tot contains for each scenario an array of mean response time for each
    # probability
    print(x_bar_vec_tot)
    print("La sotto lunghezza e': " + str(len(x_bar_vec_tot[1])))

    # How many points we have to plot
    for index_scenario in range(int(np.size(x_bar_vec_tot) / len(x_bar_vec_tot[1]))):
        step = 0.75
        starting_point = 0.25
        data_label_offset = 0.03
        x = []
        for step_index in range(len(x_bar_vec_tot[1])):
            x.append(starting_point + step * step_index)

        plt.errorbar(x, x_bar_vec_tot[index_scenario], yerr=e_vec_tot[index_scenario],
                     label=NCheck_array_label[index_scenario])
        #Size of x-axis
        plt.xlim(0, step*len(x_values))
        for prob_index in range(len(x_bar_vec_tot[index_scenario])):
            # X-label postions and string value
            # plt.xticks((0.25, 1, 1.75, 2.5, 3.25), ('0.6', '0.65', '0.70', '0.75', '0.8'))
            plt.xticks(np.arange(starting_point, step * len(x_values), step), x_values)
            plt.annotate(str(round(x_bar_vec_tot[index_scenario][prob_index], 3)),
                         xy=(x[prob_index] + data_label_offset, x_bar_vec_tot[index_scenario][prob_index]),
                         size='xx-small')

        plt.legend(loc='best', prop={'size': 3.5})

    # plt.legend(loc='lower right', prop={'size': 3.5})
    plt.title("signal = " + Info + signal1[i] + ', NC = ' + numb_of_tills)
    plt.savefig(path_result1 + "/results/" + Info + outer[i] + "_" + numb_of_tills + '.pdf')
    # plt.show()
    counter = counter + 1

'''
outer = ["WaitingTime", "ResponseTime"]
signal1 = ["_signalWaitingTimeCheckOut", "_signalResponseTimeCheckOut"]
signal2 = ["_signalWaitingTimeCheckOutAndWrap", "_signalResponseTimeCheckOutAndWrap"]

counter = 0
for i, j, k in zip(signal1, signal2, outer):
    for numberCheck in NCheck:
        plt.figure(counter)
        x_bar_vec = []
        start_vec = []
        end_vec = []
        e_vec = []

        x_bar_vec2 = []
        start_vec2 = []
        end_vec2 = []
        e_vec2 = []

        for temp in wrappingProbability:
            fileConf2_1 = open(
                 path_result + "General-n=" + numb_of_tills + ',' + "nc=" + numberCheck + ',' + initialInterArrivalRate + ',' + '1#2f' + checkoutRate + ',' + '1#2f' + wrappingRate + ',' + temp + '' + i + ".txt",
                'r')
            LinesConf2_1 = fileConf2_1.readlines()

            x_bar_vec.append(float(LinesConf2_1[1][7:-1]))
            start_vec.append(float(LinesConf2_1[2][7:-1]))
            end_vec.append(float(LinesConf2_1[3][5:-1]))
            e_vec.append(float(LinesConf2_1[3][5:-1]) - float(LinesConf2_1[2][7:-1]))

        for temp in wrappingProbability:
            fileConf2_2 = open(
                path_result + "General-n=" + numb_of_tills + ',' + "nc=" + numberCheck + ',' + initialInterArrivalRate + ',' + '1#2f' + checkoutRate + ',' + '1#2f' + wrappingRate + ',' + temp + ''+ j + ".txt",
                'r')
            LinesConf2_2 = fileConf2_2.readlines()

            x_bar_vec2.append(float(LinesConf2_2[1][7:-1]))
            start_vec2.append(float(LinesConf2_2[2][7:-1]))
            end_vec2.append(float(LinesConf2_2[3][5:-1]))
            e_vec2.append(float(LinesConf2_2[3][5:-1]) - float(LinesConf2_2[2][7:-1]))

        tmp_i = i.replace('_', '')
        tmp_j = j.replace('_', '')

        x = [0.25, 1, 1.75, 2.5, 3.25]
        plt.errorbar(x, x_bar_vec, yerr=e_vec, label = tmp_i)
        plt.xlim(0, 4)
        #X-label postions and string value
        plt.xticks((0.25, 1, 1.75, 2.5, 3.25), ('0.6', '0.65', '0.70', '0.75', '0.8'))
        plt.annotate(str(round(x_bar_vec[0], 3)), xy=(0.28, x_bar_vec[0]), size='xx-small')
        plt.annotate(str(round(x_bar_vec[1], 3)), xy=(1.03, x_bar_vec[1]), size='xx-small')
        plt.annotate(str(round(x_bar_vec[2], 3)), xy=(1.78, x_bar_vec[2]), size='xx-small')
        plt.annotate(str(round(x_bar_vec[3], 3)), xy=(2.53, x_bar_vec[3]), size='xx-small')
        plt.annotate(str(round(x_bar_vec[4], 3)), xy=(3.28, x_bar_vec[4]), size='xx-small')
        plt.legend(loc='best', prop={'size': 3.5})

        plt.errorbar(x, x_bar_vec2, yerr=e_vec2, label= tmp_j)
        plt.annotate(str(round(x_bar_vec2[0], 3)), xy=(0.28, x_bar_vec2[0]), size='xx-small')
        plt.annotate(str(round(x_bar_vec2[1], 3)), xy=(1.03, x_bar_vec2[1]), size='xx-small')
        plt.annotate(str(round(x_bar_vec2[2], 3)), xy=(1.78, x_bar_vec2[2]), size='xx-small')
        plt.annotate(str(round(x_bar_vec2[3], 3)), xy=(2.53, x_bar_vec2[3]), size='xx-small')
        plt.annotate(str(round(x_bar_vec2[4], 3)), xy=(3.28, x_bar_vec2[4]), size='xx-small')
        plt.legend(loc='lower right', prop={'size': 3.5})

        plt.title("signal = " + k + ', NC = ' + numberCheck)
        plt.savefig(path_to_save + k + "_" + numberCheck + '.pdf')
        #plt.show()
        counter = counter + 1
'''
