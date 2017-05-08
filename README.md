# pulsar_mega_master

Code for an Arduino Mega master.

For a month-long installation at the National Museum of Singapore called 'Into Pulsar' by artist Ryf Zaini. 

This version is different from the earlier Night Fest version and does not have LED lights.

Three bicycles placed at the bottom of a 'tree'. 

User cycles any of these to trigger the 28 x ebike wheels on top of the tree to go faster. 

As the user continues cycling, in a span of 20 seconds, 5 more relays will be triggered in sequence to activate the bicycle gears and sprockets to make the installation come alive progressively. 

At the end of 20 seconds, the flood lights will turn on for about 8 seconds.

The 28 ebike motor controllers are controlled via 14 PWM pins on the master mega. One pin for 2 controllers. Hence there is no client mega. PWM pins 2, ..., 13, 38, 44, 45, 46

Note: Cannot have delay() in program else rpm calculation will be affected
