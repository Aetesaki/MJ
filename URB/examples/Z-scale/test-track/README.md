This folder contains the test tracks I used during development

To make URB compatible with Z-scale add 1-3 diodes between the 12 volt
accessory output of the Märklin Z-scale controller and the input of the 
L298Ns 12V input. Use the accessory ground output of the controller as
common ground for all equipment, both railroad accessories and electronic
components of the Universal Railway Bus

In the initial test, the train will accellerate up to full speed, pass 
the inner track once, followed by the outer track, then the inner track
once more before decellerating and finally stopping at the feedback track.
The Arduino can be reset at that time to drive the train again.
No interaction is required in this test.

