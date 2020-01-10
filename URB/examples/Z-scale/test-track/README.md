This folder contains the test tracks I used during development

To make URB compatible with Z-scale add 1-3 diodes between the 12 volt
accessory output of the Märklin Z-scale controller and the input of the 
L298Ns 12V input. Use the accessory ground output of the controller as
common ground for all equipment, both railroad accessories and electronic
components of the Universal Railway Bus

In the initial test, the train will accelerate up to full speed, pass 
the inner track once, followed by the outer track, then the inner track
once more before decelerating and finally stopping at the feedback track.
The Arduino can be reset at that time to drive the train again.
-No interaction is required in this test.
-The sketch is 182 lines long
-With debugging, it compiles to 3248 bytes, using 416 bytes of RAM

In the secondary test, the train will accelerate up to full speed, pass
the outer track, decellerate, and stop on the inner track, where it will
halt for 2 seconds, and start again.
-This test is continuous
-No interaction is required.
-The sketch is 1043 lines long (5.7 times longer than the initial test)
-Without debugging, it compiles to 4078 bytes, using 279 bytes of RAM
-With debugging, it compiles to 5028 bytes, using 468 bytes of RAM 
(Debugging is available in the sketch)


