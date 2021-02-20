*****THE INPUT comes from "MB.txt" which has the lat/long coordinate pair for the point of beginning, followed by all the metes 
and bounds calls in the following format:

    [POB_latitude_in decimal degrees]  [POB_longitude_in decimal degrees]

    [NorthSouthBearing] [Degrees] [Minutes] [Seconds] [EastwestBearing] [Distance]
    [NorthSouthBearing] [Degrees] [Minutes] [Seconds] [EastwestBearing] [Distance]
    [NorthSouthBearing] [Degrees] [Minutes] [Seconds] [EastwestBearing] [Distance]
    [NorthSouthBearing] [Degrees] [Minutes] [Seconds] [EastwestBearing] [Distance]

So a line might be:
    N     32     34     17   W    243.67

meaning "North 32 degrees, 34 minutes, 17 seconds west, 243.67 feet (or whatever unit is specified at runtime)"

"N" can be alternately represented as "n" or "8"; "S" as "s" or "2"; "W" as "w" or "4" and "E" as "e" or "6"
The number substitutes exist to facilitate ultra-fast keypad entries that don't require much hand movement.

*****THE OUTPUT is KML.  The output kml has 3 layers, one visible by default and the other two turned off by default.  
The tract is visible and the two that are off by default are a point highlighting the point of beginning and a 
line showing the first call.  This is so that users can see where the point of beginning is and which direction the 
calls start out in.  This is for debugging the metes and bounds in the MB.txt file.

*****THE WORKFLOW is such that I usually pick a point of beginning that is close, but not perfect.  Then, once I have
the shape of the tract drawn, I adjust the POB.  This is made easier by the POB and first call layers of the kml. 

****THE MISSING FEATURES, in my opinion are as follows: Obviously, forkers can do as they please, but I'd love to merge 
some of the following functionality into the main branch if someone is kind enough to help!:
