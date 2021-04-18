import os
import sys
import numpy as np
from matplotlib import pyplot as plt

from Util import JSONSerializer

app_path = os.path.dirname(os.path.realpath('ProjectBSPlots.py'))
path_to_data = os.path.join(app_path, '..', 'ProjectBosmaSmeets', 'debug')

config_filename = os.path.join(path_to_data, 'bsplotsconfig.json')
data_filename = os.path.join(path_to_data, 'bsplotsdata.json')

print('Reading saved configuration and data...')

config = JSONSerializer.DeserializeJSON(config_filename)
all_data = JSONSerializer.DeserializeJSON(data_filename)

M = config['M']
N = config['N']
D = config['D']
show_optimalCF = config['show_optimalCF']
allow_dups = config['allow_duplicates']

if len(D) == 1 :
    D = [D[0]] * (len(M))

optimal_cf_data = all_data['Optimal_CF'] if show_optimalCF else []
dup_data = all_data['Dup_Data'] if allow_dups else []
nondup_data = all_data['Nondup_Data']

print('Plotting data...')

legend_arr = []
z_vector = nondup_data[0]
ms = 3
if show_optimalCF :
    plt.plot(optimal_cf_data[0], optimal_cf_data[1], 'ro', markersize=ms)
    legend_arr.append('Optimal CF')

for i in range(1, len(nondup_data)) :
    legend_arr.append('m = %d, n = %d, d = %d' % (M[i - 1], N[i - 1], D[i - 1]))
    plt.plot(nondup_data[0], nondup_data[i], 'go', markersize=ms, marker= i - 1)

    if allow_dups :
        plt.plot(dup_data[0], dup_data[i], 'bo', markersize=ms, marker= i - 1)


plt.legend(legend_arr)
plt.show()