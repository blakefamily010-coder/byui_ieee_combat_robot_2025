# client information for controller server

every __n__ seconds
recieve input from controler -> send get request to esp32

## routes

### /control/a_button

rising edge of the a_button

### /control/a_button_off

falling edge of the a_button

### /control/b_button

rising edge of the b_button

### /control/b_button_off

falling edge of the b_button

### /control/x_button

rising edge of the x_button

### /control/x_button_off

falling edge of the x_button

### /control/y_button

rising edge of the y_button

### /control/y_button_off

falling edge of the y_button

### /control/r_bumper

rising edge of the right bumper

### /control/r_bumper_off

falling edge of the right bumper

### /control/l_bumper

rising edge of the left bumper

### /control/l_bumper_off

falling edge of the left bumper

### /control/r_trigger

rising edge of the right trigger

### /control/r_trigger_off

falling edge of the right trigger

### /control/l_trigger

rising edge of the left trigger

### /control/l_trigger_off

falling edge of the left trigger

### /control/l_stick/*x*/*y*

every __n__ seconds, gives the x and y of the l_stick

### /control/r_stick/*x*/*y*

every __n__ seconds, gives the x and y of the r_stick
