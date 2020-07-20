import sys
import numpy
from Util import JSONSerializer

filename = "C:\\Users\\jinge\\source\\MUSE\\CPP\\Debug\\test.json"

json = JSONSerializer.DeserializeJSON(filename)

print('Count: ', json['Count'])
print('Op Count: ', json['OpCount'])
print('q Sequence: ', json['qSequence'])

