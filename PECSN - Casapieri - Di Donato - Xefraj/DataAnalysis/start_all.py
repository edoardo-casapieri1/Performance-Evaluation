import os
import json

with open('config.json') as f:
    config_parameters = json.load(f)

paths1 = config_parameters['paths_conf1']
paths2 = config_parameters['paths_conf2']

for path in paths1:
    if not os.path.exists(path):
        os.makedirs(path)
    if not os.path.exists(path + "/results"):
        os.makedirs(path+"/results")

for path in paths2:
    if not os.path.exists(path):
        os.makedirs(path)
    if not os.path.exists(path + "/results"):
        os.makedirs(path + "/results")

os.chdir("C:/Users/Riccardo/PycharmProjects/Python_performance_evaluation")

'''
os.system("python gen_csv_conf1.py")
os.system("python gen_csv_conf2.py")

os.system("python Response_Time_And_Wrap_1.py")
os.system("python Response_Time_And_Wrap_2.py")
'''
os.system("python MatrixGenerator_conf1.py")
os.system("python MatrixGenerator_conf2.py")


os.system("python varing_factors.py 0")
os.system("python varing_factors.py 1")
os.system("python varing_factors.py 2")



os.system("python varing_factors.py 3")

os.system("python Lorenz_curve.py")

