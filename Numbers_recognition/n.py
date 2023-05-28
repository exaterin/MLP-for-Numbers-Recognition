# Read the input file
with open('Numbers_recognition/notcentered_numbers.txt', 'r') as file:
    data = file.read()

# Remove newline characters
data = data.replace('\n', '')

# Split the values by comma
values = data.split(',')

# Multiply each value by 250 and convert to integers
multiplied_values = [int(float(value) * 250) for value in values]

# Group the multiplied values into sets of 785
grouped_values = [multiplied_values[i:i+784] for i in range(0, len(multiplied_values), 784)]

# Convert each set into a string with comma-separated values
modified_data = '\n'.join(','.join(str(value) for value in group) for group in grouped_values)

# Write the modified data back to the file
with open('Numbers_recognition/output.txt', 'w') as file:
    file.write(modified_data)