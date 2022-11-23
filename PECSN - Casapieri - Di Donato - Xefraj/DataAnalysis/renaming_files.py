import os

path = "F:/Omnet/Conf1-12"
for count, filename in enumerate(os.listdir(path)):
    if "0.70" in filename:
        print("Renaming " + filename)
        new_file_name = filename.replace("0.70", "0.7")
        os.rename(path + "/" + filename, path + "/" + new_file_name)
