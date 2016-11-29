import json
import os

# parse ddl.json file
with open('DDL.json') as ddl_file:
    ddl = json.load(ddl_file)
ddl_file.close

for device in ddl['Devices']:
	device_name  =  device['Description']['Name']
	device_type  =  device['Description']['Device_Type']

	if device_type == 'Sensor':
		# generate read function
		for signal_item in device['Interface']['Signal']:
			signal_id = signal_item['id']
			for reading_item in device['Interface']['Reading']:
				if signal_id != reading_item['From']:
					continue
				expression = reading_item['Computation']['Expression']
				return_data_type = reading_item['Data_Type']
				function_name = "../device/" + device_name + "/" + device_name + 'read' + '.c'
				read_sensor = open(function_name, 'w')
				read_sensor.write('#include <xinu.h>\n\n\n')
				read_sensor.write('devcall' + ' ' + device_name + 'read' + '(struct dentry* devptr, void* buff, uint32 count)\n')
				read_sensor.write('{\n')
				return_len = reading_item['Return_Length']
				read_sensor.write('    ' + 'uint32 len = ' + return_len + ';\n')
				read_sensor.write('    ' + return_data_type + ' result[len];\n')
				read_sensor.write('    ' + 'uint32 retval;\n')
				read_sensor.write('    ' + 'char ret[count];\n')
				read_sensor.write('    ' + 'retval = '+'read' + device_name + '(devptr, (' + return_data_type + ' *)&'+'result)' + ';\n')
				for item in expression:
					if item != '':
						transform = item.replace(signal_id, 'result')
						read_sensor.write('    ' + transform + ';\n')
				next_line = 'sprintf(ret, "'
				i = 0
				while i < int(return_len) - 1:
					i = i + 1
					next_line = next_line + '%d '
				next_line = next_line + '%d", '
				i = 0
				while i < int(return_len) - 1:
					next_line = next_line + 'result[' + str(i) + '],'
					i = i + 1
				next_line = next_line + 'result[' + str(i) + ']);'
                read_sensor.write('    ' + next_line + '\n')
                read_sensor.write('    ' + 'memcpy((char *)buff,(char *)ret,count);\n')
                read_sensor.write('    ' + 'return retval;\n')
                read_sensor.write('}')
                read_sensor.close()

		 

	if device_type == 'Actuator':     # other device type
	    # generate read function_name
	    for signal_item in device['Interface']['Signal']:
	    	signal_id = signal_item['id']
	    	for reading_item in device['Interface']['Reading']:
	    		if signal_id != reading_item['From']:
	    			continue
	    		return_data_type = reading_item['Data_Type']
	    		function_name = "../device/" + device_name + "/" + device_name + 'read' + '.c'
	    		read_actuator = open(function_name, 'w')
	    		read_actuator.write("#include <xinu.h>\n\n\n")
	    		read_actuator.write('devcall' + ' ' + device_name + 'read' + '(struct dentry* devptr, void* buff, uint32 count)\n')
	    		read_actuator.write('{\n')
	    		return_len = reading_item['Return_Length']
	    		read_actuator.write('    ' + 'uint32 len = ' + return_len + ';\n')
	    		read_actuator.write('    ' + return_data_type + ' result[len];\n')
	    		read_actuator.write('    ' + 'uint32 retval;\n')
	    		read_actuator.write('    ' + 'char ret[count];\n')
	    		read_actuator.write('    ' + 'retval = ' + 'read' + device_name + '(devptr, (' + return_data_type + ' *)&'+'result)' + ';\n')
	    		for item in expression:
					if item != '':
						transform = item.replace(signal_id, 'result')
						read_sensor.write('    ' + transform + ';\n')
                next_line = 'sprintf(ret, "'
                i = 0
                while i < int(return_len) - 1:
	    			i = i + 1
	    			next_line = next_line + '%d '
                next_line = next_line + '%d", '
                i = 0
                while i < int(return_len) - 1:
	    			next_line = next_line + 'result[' + str(i) + '],'
	    			i = i + 1
                next_line = next_line + 'result[' + str(i) + ']);'
                read_actuator.write('    ' + next_line + '\n')
                read_actuator.write('    ' + 'memcpy((char *)buff,(char *)ret,count);\n')
                read_actuator.write('    ' + 'return retval;\n')
                read_actuator.write('}')
                read_actuator.close()

                function_name = "../device/" + device_name + "/" + device_name + 'write' + '.c'
                write_actuator = open(function_name, 'w')
                write_actuator.write("#include <xinu.h>\n\n\n")
                write_actuator.write('devcall' + ' ' + device_name + 'write' + '(struct dentry* devptr, void* buff, uint32 count)\n')
                write_actuator.write('{\n')
                write_actuator.write('    ' + 'uint32 retval;\n')
                write_actuator.write('    ' + 'retval = ' + 'write' + device_name + '(devptr, (' + return_data_type + '*) ' + 'buff);\n')
                write_actuator.write('    ' + 'return retval;\n')
                write_actuator.write('}')
                write_actuator.close()

	    		
