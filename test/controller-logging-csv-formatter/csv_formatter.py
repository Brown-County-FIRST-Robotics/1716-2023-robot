import csv

new_data = []
cols = [0]

filename = input('CSV path: ')

with open(filename, 'r') as input_file:
    reader = csv.reader(input_file)
    
    header = next(reader) #separate the header for reading
    
    #find the columns that start with `network_table:///Shuffleboard/Controller/` and store int in an array
    for i in range(len(header)):
        if "network_table:///Shuffleboard/Controller/" in header[i]:
            cols.append(i)

with open(filename, 'r') as input_file:
    reader = csv.reader(input_file)

    for row in reader:
        kept_cols = []
        for i in cols:
            kept_cols.append(row[i])
        new_data.append(kept_cols)
        
with open('output.csv', 'w') as output_file:
    writer = csv.writer(output_file)
    
    for row in new_data:
        writer.writerow(row)