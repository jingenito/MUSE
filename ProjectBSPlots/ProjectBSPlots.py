import os, sys
import numpy as np
from matplotlib import pyplot as plt

from Util import JSONSerializer

app_path = os.path.dirname(os.path.realpath('ProjectBSPlots.py'))
path_to_data = os.path.join(app_path, '..', 'ProjectBosmaSmeets', 'debug')

file1 = os.path.join(path_to_data, 'freqs.json')
file2 = os.path.join(path_to_data, 'freqs_nonrepeated.json')
file3 = os.path.join(path_to_data, 'F.json')

print('Deserializing ' + file1)
json1 = JSONSerializer.DeserializeJSON(file1)
print('Deserializing ' + file2)
json2 = JSONSerializer.DeserializeJSON(file2)
print('Deserializing ' + file3)
json3 = JSONSerializer.DeserializeJSON(file3)

zvec = json1['z']
freqs = json1['v']
freqs_nonrepeated = json2['v']
fvec = json3['v']

print('Plotting data...')

ms = 1
plt.plot(zvec,freqs,'go', markersize=ms)
plt.plot(zvec,freqs_nonrepeated,'bo', markersize=ms)
plt.plot(zvec,fvec,'ro', markersize=ms)
plt.legend(['theta with duplicates','theta without duplicates','optimal CF'])
plt.show()

#file_data = []
#for i in range(len(zvec)) :
#    if zvec[i] != 0 :
#        file_data.append(str(zvec[i]) + ' ' + str(fvec[i]) + ' ' + str(freqs[i]) + ' ' + str(freqs_nonrepeated[i]) + '\n')

#new_filename = 'C:\\Users\\jinge\\Downloads\\data.txt'
#print('Saving data to ' + new_filename)

#new_file = open(new_filename, 'w') 
#new_file.writelines(file_data)

#print('Done.')