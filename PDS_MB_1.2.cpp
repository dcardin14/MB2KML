#include<iostream>
#include<string>
#include<fstream>
#include <iomanip>
#include<math.h>
#include<ctime>
#include<cstdlib>
using namespace std;

//New Comment to test GitHub

//  This is a new comment that I'm going to commit.
double get_lat_ratio(double);
double get_long_ratio(double);

bool authenticate_license();

double cumulative_square_footage = 0,cumulative_acreage = 0, previous_y = 0;
int array_size;
//  05-27-2014 DC:  I don't know what this stuff is.  I got it from http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int main()
{
    //Instatiate the ifstream class with an object named inputFile
    ifstream inputFile;
    ofstream outputFile;

                // Declare constants
                const double PI = 3.14159265358979323;


    // Declare variables to hold the input read from the file
    string NSbearing, EWbearing, units, unit_Choice, oil_and_gas_play;
    double Degrees, Minutes, Seconds, distance, POBlat,POBlong, lat, long_, decimalDegrees, azimuthDegrees, aRadians, hypotenuseInFeet, xInFeetToBeAdded,
                                                yInFeetToBeAdded, xInDegreesOfLongToBeAdded, yInDegreesOfLatToBeAdded, xratio, yratio;



                    while(authenticate_license() == 0)
					{
						cout << "Your license for this product has expired.  Please contact \nDaniel Cardin at (720) 440-1741 or dcardin@cardinoil.com \nfor a renewal.\n\n";
						system("pause");
                    }


				cout << "What units are used in your data?\n\n";
				cout << "(f) Feet\n";
				cout << "(v) Varas\n";
				cout << "(r) Rods\n";
				cout << "(c) Chains\n";
				cout << "(p) Poles\n";
				cout << "(y) Yards\n";
                cin >> unit_Choice;



    //
    inputFile.open("MB.txt");
    cout << "SOFTWARE BY DANIEL CARDIN\nCOPYRIGHT 2013\nALL RIGHTS RESERVED\n\n";

    //      05-29-2014 DC:  I'm going to loop through the MB file twice:  Once to count the number of MB calls to use as the size of an array to put the
    //      lat/longs in and the second time to calculate the acreage of the resulting polygon so I can use that data in the KML to increase
    //      the richness of my content.  What's unfortunate is that I don't seem to be calculating acreage exactly correctly.  More complicated
    //      polygons are off by 3 or 4 percent on acreage, which is obviously unnacceptable long term.  I suspect it's some kind of rounding error.

	inputFile >> POBlat >> POBlong;


if(POBlat < 25 || POBlat > 50 || POBlong > -60 || POBlong < -125)  //  Check for out of bounds
{

	cout << "Your data appears to have a Point of Beginning that is outside the Continental \nUnited States.  ";
	cout << "If this is an error, please correct the POB and rerun the \nprogram. ";
	cout <<  "If it's not an error, please contact Petroleum Data Services, Inc. \nfor an international license.\n\n\n\n";
	system("pause");

}

//05-22-2014 DC:  Make sure that it doesn't matter if the person tries to forget the negative sign before the longitude
if(POBlong > 0)
{
	POBlong = 0-POBlong;
}

//  Now set the x_long_degrees_per_foot and Y_lat_degrees_per_foot based on the area
// First I'll initialize the values to zero
				xratio = 0;
				yratio = 0;

				xratio = get_long_ratio(POBlat);  //They both take POB lat as the argument.  Weird but that is how it is
				yratio = get_lat_ratio(POBlat);  //They both take POB lat as the argument.  Weird but that is how it is

	array_size = 0;
    while(!inputFile.eof())
	{
    	inputFile >> NSbearing >> Degrees >> Minutes >> Seconds >> EWbearing >> distance;
    	array_size++;
    }
    cout << "Your array size is: " << array_size << ". " << endl;
    double x_array[array_size], y_array[array_size];  // 05-29-2014 DC:  Create parallel arrays to hold x and y values
    inputFile.close();  //  05-29-2014 DC:  This is so I can start back at the beginning.  There's probably a better way to reset the cursor...
    inputFile.open("MB.txt");
    //  05-29-2014 DC:  Now loop through and actually put the values in the array.
    inputFile >> POBlat >> POBlong;
    x_array[0] = POBlong;
    y_array[0] = POBlat;

	for(int count = 0; count < array_size-1; count++)
	{
	  // Get the metes and bounds calls from the MB file
	  inputFile >> NSbearing >> Degrees >> Minutes >> Seconds >> EWbearing >> distance;

        //calculate the decimal degrees from the degrees, minutes, and seconds in the deed
        decimalDegrees = Degrees + (Minutes/60)+(Seconds/60);

                                                //This set of nested "if" statements uses the bearing calls to turn the degrees into azimuth degrees
                                                if (NSbearing == "N" || NSbearing == "n" || NSbearing == "8")
                                                {
                                                                if (EWbearing == "E" || EWbearing =="e" || EWbearing == "6")
                                                                {
                                                                                azimuthDegrees = decimalDegrees;

                                                                }
                                                                else if (EWbearing == "W" || EWbearing == "w" || EWbearing == "4")
                                                                {
                                                                                azimuthDegrees = 360-decimalDegrees;
                                                                }
                                                                //trailing else to handle incorrect input? (add later)
                                                }
                                                else if (NSbearing == "S" || NSbearing == "s" || NSbearing == "2")
                                                {

                                                                if (EWbearing == "E" || EWbearing == "e" || EWbearing == "6")
                                                                {
                                                                                azimuthDegrees = 180-decimalDegrees;

                                                                }
                                                                else if (EWbearing == "W" || EWbearing == "w" || EWbearing == "4")
                                                                {
                                                                                azimuthDegrees = 180+decimalDegrees;
                                                                }
                                                                //trailing else to handle incorrect input? (add later)

                                                }
                                                //trailing else to handle incorrect input? (add later)


                                                //convert azimuthDegrees into radians
                                                aRadians = azimuthDegrees * PI/180.0;

                                                //find out how long the vector call is in feet (regardless of whether the call itself is in feet or varas)
                                              	//05-24-2014  I'm not too sure about putting all this in the loop, but I'm
												 //going to try it
												if(unit_Choice == "f"|| unit_Choice == "F"){hypotenuseInFeet = distance;}
												if(unit_Choice == "v"|| unit_Choice == "V"){hypotenuseInFeet = distance * 2.77778333333;}
												if(unit_Choice == "r"|| unit_Choice == "R"){hypotenuseInFeet = distance * 16.5;}
												if(unit_Choice == "c"|| unit_Choice == "C"){hypotenuseInFeet = distance * 66;}
												if(unit_Choice == "p"|| unit_Choice == "P"){hypotenuseInFeet = distance * 16.5;}
												if(unit_Choice == "y"|| unit_Choice == "Y"){hypotenuseInFeet = distance * 3;}

                                                //calculate the x and y in feet to be added
                                                xInFeetToBeAdded = sin(aRadians) * hypotenuseInFeet;
                                                yInFeetToBeAdded = cos(aRadians) * hypotenuseInFeet;

                                                //calculate the y(lat) and x(long) to be added
                                                xInDegreesOfLongToBeAdded = (xInFeetToBeAdded * xratio);
                                                yInDegreesOfLatToBeAdded = (yInFeetToBeAdded * yratio);

    x_array[(count + 1)] = x_array[(count)] + xInDegreesOfLongToBeAdded;  //  05-29-2014  DC:  Put the x coordinate into an array element whose index is set by the count.
    y_array[(count + 1)] = y_array[(count)] + yInDegreesOfLatToBeAdded;  //  05-29-2014  DC:  Put the y coordinate into an array element whose index is set by the count.

	}
	x_array[array_size] = POBlong;  //The last x in the array should be the x for the POB
	y_array[array_size] = POBlat;   //The last x in the array should be the x for the POB

    inputFile.close();  // 05-29-2014 DC:  Start again to reset the cursor.
    inputFile.open("MB.txt");
    system("pause");

    //05-29-2014 DC:  Now I'm going to calculate the acreage.  I will eventually offload this functionality to a function, but I'm not
    //  perfectly confident about passing arrays to functions, so I'm going to wait until I have this working first.

    /*This part is the KML code.  I may need to have the while loop kind of in the middle.*/
	outputFile.open("Generated.kml");
	//This is my actual kml.  Notice that I have the escape character before all the quotes that actually neet to be part of the kml itself
	outputFile           							<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                               	<< "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n"

												<<  "<Document>\n"
													<<  "<name>";
	//  05-27-2014 DC:  This should add created date to my kml name (2nd level)
	outputFile << currentDateTime() << "</name>\n"
													<<  "<Style id=\"s_ylw-pushpin_hl\">\n"
														<<  "<IconStyle>\n"
															<<  "<scale>1.3</scale>\n"
															<<  "<Icon>\n"
																<<  "<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n"
															<<  "</Icon>\n"
															<<  "<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n"
														<<  "</IconStyle>\n"
														<<  "<LineStyle>\n"
															<<  "<color>ffffaa00</color>\n"
														<<  "</LineStyle>\n"
														<<  "<PolyStyle>\n"
															<<  "<color>59ff5500</color>\n"
														<<  "</PolyStyle>\n"
													<<  "</Style>\n"
	///////////////////////////////////////////////////////I'm adding this right now 05-22-2014
														<< "<Style id=\"failed\">"
														<<" <IconStyle>"
														<<" <color>ff0000ff</color>"
														<<" 	<scale>0.5</scale>"
														<<" 	<Icon>"
														<<" 	<href>http://maps.google.com/mapfiles/kml/shapes/donut.png</href>"
														<<" 	</Icon>"
														<<" 	</IconStyle>"
														<<" 	<LabelStyle>"
														<<" 	<scale>0.5</scale>"
														<<" 	</LabelStyle>"
														<<" 	<ListStyle>"
														<<" 	</ListStyle>"
														<<" 	</Style>"
														<< "<StyleMap id=\"failed0\">"
														<<" 	<Pair>"
														<<" 	<key>normal</key>"
														<<" 	<styleUrl>#failed</styleUrl>"
														<<" 	</Pair>"
														<<" 	<Pair>"
														<<" 	<key>highlight</key>"
														<<" 	<styleUrl>#failed1</styleUrl>"
														<<" 	</Pair>"
														<<" 	</StyleMap>"
														<<"	<Style id=\"failed1\">"
														<<"<IconStyle>"
														<<"	<color>ff0000ff</color>"
														<<"	<scale>0.5</scale>"
														<<"	<Icon>"
														<<"		<href>http://maps.google.com/mapfiles/kml/shapes/donut.png</href>"
														<<"	</Icon>"
														<<"</IconStyle>"
														<<"<LabelStyle>"
														<<"	<scale>0.5</scale>"
														<<"</LabelStyle>"
														<<"<ListStyle>"
														<<"</ListStyle>"
														<<"</Style>"
	///////////////////////////////////////////////////////////OK that's the end of what i added 05-22-2014
														<<  "<StyleMap id=\"m_ylw-pushpin0\">\n"
														<<  "<Pair>\n"
															<<  "<key>normal</key>\n"
															<<  "<styleUrl>#s_ylw-pushpin0</styleUrl>\n"
														<<  "</Pair>\n"
														<<  "<Pair>\n"
															<<  "<key>highlight</key>\n"
															<<  "<styleUrl>#s_ylw-pushpin_hl</styleUrl>\n"
														<<  "</Pair>\n"
													<<  "</StyleMap>\n"
													<<  "<Style id=\"s_ylw-pushpin0\">\n"
														<<  "<IconStyle>\n"
															<<  "<scale>1.1</scale>\n"
															<<  "<Icon>\n"
																<<  "<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>\n"
															<<  "</Icon>\n"
															<<  "<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>\n"
														<<  "</IconStyle>\n"
														<<  "<LineStyle>\n"
															<<  "<color>59ff5500</color>\n"
														<<  "</LineStyle>\n"
														<<  "<PolyStyle>\n"
															<<  "<color>59ff5500</color>\n"
														<<  "</PolyStyle>\n"
													<<  "</Style>\n"
													<<  "<Placemark>\n"
														<<  "<name>PDS Metes and Bounds!</name>\n"
														<< "<description>Daniel Cardin                   \nPetroleum Data Services, Inc.\n(720) 440-1741 \ndcardin@cardinoil.com</description>"
														<<  "<styleUrl>#m_ylw-pushpin0</styleUrl>\n"
														<<  "<Polygon>\n"
															<<  "<tessellate>1</tessellate>\n"
															<<  "<outerBoundaryIs>\n"
																<<  "<LinearRing>\n"
																	<<  "<coordinates>\n";
	////////////////////////////////////////THIS IS WHERE ALL THE LATLONG PAIRS GO
                //
//                 // put the POB lat/long pairs in there BEFORE the loop starts to get all the rest of them
//                lat = POBlat;
//                long_ = POBlong;
//
//    outputFile       << setprecision(13) << showpoint << fixed << long_ << "," << lat << ",0" << endl;
//    cout                   << setprecision(13) << showpoint << fixed << long_ << "," << lat << ",0" << endl;
//    //  Notice that the long is first in the kml, NOT LAT
//
//
    //Read in the metes and bounds calls from the file
    for(int count = 0; count < array_size; count++)
    {
       // Get the coordinates from the array
       lat = y_array[count];
       long_ = x_array[count];
       outputFile << setprecision(13) << showpoint << fixed << long_ << "," << lat << ",0" << endl;


    }

	//  Now that the loop is done emitting points from the metes and bounds calls (i.e. the end of the MB file has been reached)
	//  I'm going to arbitrarily add one last point:  The POB.  This way polygons will always close.
	outputFile << setprecision(13) << showpoint << fixed << POBlong << "," << POBlat << ",0" << endl;
	outputFile           << "</coordinates>\n"
                                                << "</LinearRing>\n"
                                                << "</outerBoundaryIs>\n"
                                                << "</Polygon>\n"
                                                << "</Placemark>\n"
                                                //05-22-2014 DC:  this next stuff I'm adding (except the last 2 tags to close out the kml
                                                //i want to placemark the pob
                                                << "<Placemark>"
												<< "<name>Point of Beginning</name>"
												<< "<visibility>0</visibility>"

												<< "	<LookAt>"
												<< "		<longitude>" << POBlong << "</longitude>"
												<< "		<latitude>" << POBlat << "</latitude>"
												<< "		<altitude>0</altitude>"
												<< "		<heading>-0.001715575336961538</heading>"
												<< "		<tilt>6.314268927210937</tilt>"
												<< "		<range>1193.979515540014</range>"
												<< "		<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>"
												<< "	</LookAt>"
												//<< "	<styleUrl>#m_ylw-pushpin</styleUrl>"
												<< "<styleUrl>#failed0</styleUrl>"
												<< "	<Point>"
												<< "		<gx:drawOrder>1</gx:drawOrder>"
												<< "		<coordinates>" << POBlong << "," << POBlat << ",0</coordinates>"
												<< "	</Point>"
												<< "</Placemark>"
												//05-23-2014 DC: OK now I'm going to make it show the first deed call.
												<< "<Placemark>"
												<< "<name>First Deed Call</name>"
												<< "<visibility>0</visibility>"
												<< "<styleUrl>#msn_ylw-pushpin</styleUrl>"
												<< "<LineString>"
												<< "<tessellate>1</tessellate>"
												<< "<coordinates>";
	//05-23-2014 DC:  Here I'm going to use some of my generation code from above to create a line fromm the first call
	//This will let people know which direction the calls are going in so they can debug their data entry.
	//First I guess I have to close and then reopen the input file so I can start back at the beginning....?  (I wonder if there's a better way to do this...)
				inputFile.close();
				inputFile.open("MB.txt");

				inputFile >>  POBlat >> POBlong;

    //05-22-2014 DC:  Make sure that it doesn't matter if the person tries to forget the negative sign before the longitude
                 if(POBlong > 0)
				{
					POBlong = 0-POBlong;
				}


                 // put the POB lat/long pairs in there first (this time for the first call line string)
                lat = POBlat;
                long_ = POBlong;

    			outputFile << setprecision(13) << showpoint << fixed << long_ << "," << lat << ",0" << endl;
   	//05-23-2014 DC:  Get the first call (I'm not using the while loop this time because it's one and only one call.)
                  inputFile >> NSbearing >> Degrees >> Minutes >> Seconds >> EWbearing >> distance;
				 decimalDegrees = Degrees + (Minutes/60)+(Seconds/60);

                                                //This set of nested "if" statements uses the bearing calls to turn the degrees into azimuth degrees
                                                if (NSbearing == "N" || NSbearing == "n" || NSbearing == "8")
                                                {
                                                                if (EWbearing == "E" || EWbearing =="e" || EWbearing == "6")
                                                                {
                                                                                azimuthDegrees = decimalDegrees;

                                                                }
                                                                else if (EWbearing == "W" || EWbearing == "w" || EWbearing == "4")
                                                                {
                                                                                azimuthDegrees = 360-decimalDegrees;
                                                                }
                                                                //trailing else to handle incorrect input? (add later)
                                                }
                                                else if (NSbearing == "S" || NSbearing == "s" || NSbearing == "2")
                                                {

                                                                if (EWbearing == "E" || EWbearing == "e" || EWbearing == "6")
                                                                {
                                                                                azimuthDegrees = 180-decimalDegrees;

                                                                }
                                                                else if (EWbearing == "W" || EWbearing == "w" || EWbearing == "4")
                                                                {
                                                                                azimuthDegrees = 180+decimalDegrees;
                                                                }
                                                                //trailing else to handle incorrect input? (add later)

                                                }
                                                //trailing else to handle incorrect input? (add later)


                                                //convert azimuthDegrees into radians
                                                aRadians = azimuthDegrees * PI/180.0;


//                                                //find out how long the vector call is in feet (regardless of whether the call itself is in feet or varas)
//                                                if (varasOrFeet == "varas" || varasOrFeet == "Varas" || varasOrFeet == "VARAS")
//                                                {
//                                                                hypotenuseInFeet = distance * 2.77778333333;
//                                                }
//                                                else
//                                                {
//                                                                hypotenuseInFeet = distance;
//                                                }
												if(unit_Choice == "f"|| unit_Choice == "F"){hypotenuseInFeet = distance;}
												if(unit_Choice == "v"|| unit_Choice == "V"){hypotenuseInFeet = distance * 2.77778333333;}
												if(unit_Choice == "r"|| unit_Choice == "R"){hypotenuseInFeet = distance * 16.5;}
												if(unit_Choice == "c"|| unit_Choice == "C"){hypotenuseInFeet = distance * 66;}
												if(unit_Choice == "p"|| unit_Choice == "P"){hypotenuseInFeet = distance * 16.5;}
												if(unit_Choice == "y"|| unit_Choice == "Y"){hypotenuseInFeet = distance * 3;}
//
                                                //calculate the x and y in feet to be added
                                                xInFeetToBeAdded = sin(aRadians) * hypotenuseInFeet;
                                                yInFeetToBeAdded = cos(aRadians) * hypotenuseInFeet;

                                                //calculate the y(lat) and x(long) to be added
                                                xInDegreesOfLongToBeAdded = (xInFeetToBeAdded * xratio);
                                                yInDegreesOfLatToBeAdded = (yInFeetToBeAdded * yratio);

                                //Get the next set of lat longs  This time it will only happen once, so I'm not doing all that "if EOF" stuff
								lat = lat + yInDegreesOfLatToBeAdded;
								long_ = long_ + xInDegreesOfLongToBeAdded;
                                //This writes it to the actual kml
                                outputFile << setprecision(13) << showpoint << fixed << long_ << "," << lat << ",0" << endl;



								outputFile		<< "</coordinates>"
												<< "</LineString>"
												<< "</Placemark>"
                                                << "</Document>\n"
                                                << "</kml>\n";

    cout << "xratio: " << xratio << "   yratio:  " << yratio << endl;
    cumulative_acreage = (cumulative_square_footage / 43560);
	if(cumulative_acreage < 0)
	{
		cumulative_acreage = (cumulative_acreage * -1);
    }
    cout << setprecision(2) << "Your calculated acreage is: " << cumulative_acreage << endl;

	system("pause");
    return 0;

    }

double get_long_ratio(double POBlat)

{

//  Now I'll test for area to decide what ratio to use.
//  Note that both lat AND long are dependent on the Y coordinate and
//  they are pretty much constant at a constant Y even x changes greatly...weird
//  because I would have thought that has only true of horizontal lines.

double x_long_degrees_per_foot = 0;
if(POBlat > 25 && POBlat < 25.2){x_long_degrees_per_foot = 3.02426366740358E-06;}
if(POBlat >= 25.2 && POBlat < 25.4){x_long_degrees_per_foot = 3.02922903091934E-06;}
if(POBlat >= 25.4 && POBlat < 25.6){x_long_degrees_per_foot = 3.03425675880693E-06;}
if(POBlat >= 25.6 && POBlat < 25.8){x_long_degrees_per_foot = 3.03932891617531E-06;}
if(POBlat >= 25.8 && POBlat < 26){x_long_degrees_per_foot = 3.04445513386469E-06;}
if(POBlat >= 26 && POBlat < 26.2){x_long_degrees_per_foot = 3.04963587347671E-06;}
if(POBlat >= 26.2 && POBlat < 26.4){x_long_degrees_per_foot = 3.05487160374649E-06;}
if(POBlat >= 26.4 && POBlat < 26.6){x_long_degrees_per_foot = 3.06016280066099E-06;}
if(POBlat >= 26.6 && POBlat < 26.8){x_long_degrees_per_foot = 3.06551934495983E-06;}
if(POBlat >= 26.8 && POBlat < 27){x_long_degrees_per_foot = 3.0709229658974E-06;}
if(POBlat >= 27 && POBlat < 27.2){x_long_degrees_per_foot = 3.07638352658149E-06;}
if(POBlat >= 27.2 && POBlat < 27.4){x_long_degrees_per_foot = 3.08190153324601E-06;}
if(POBlat >= 27.4 && POBlat < 27.6){x_long_degrees_per_foot = 3.08747750000772E-06;}
if(POBlat >= 27.6 && POBlat < 27.8){x_long_degrees_per_foot = 3.09311194900077E-06;}
if(POBlat >= 27.8 && POBlat < 28){x_long_degrees_per_foot = 3.09880541051425E-06;}
if(POBlat >= 28 && POBlat < 28.2){x_long_degrees_per_foot = 3.10454878487961E-06;}
if(POBlat >= 28.2 && POBlat < 28.4){x_long_degrees_per_foot = 3.11036185949873E-06;}
if(POBlat >= 28.4 && POBlat < 28.6){x_long_degrees_per_foot = 3.11623558741041E-06;}
if(POBlat >= 28.6 && POBlat < 28.8){x_long_degrees_per_foot = 3.12216078503611E-06;}
if(POBlat >= 28.8 && POBlat < 29){x_long_degrees_per_foot = 3.12815748396037E-06;}
if(POBlat >= 29 && POBlat < 29.2){x_long_degrees_per_foot = 3.13420673227606E-06;}
if(POBlat >= 29.2 && POBlat < 29.4){x_long_degrees_per_foot = 3.14031886797785E-06;}
if(POBlat >= 29.4 && POBlat < 29.6){x_long_degrees_per_foot = 3.14649449048815E-06;}
if(POBlat >= 29.6 && POBlat < 29.8){x_long_degrees_per_foot = 3.15273420874253E-06;}
if(POBlat >= 29.8 && POBlat < 30){x_long_degrees_per_foot = 3.15903864136066E-06;}
if(POBlat >= 30 && POBlat < 30.2){x_long_degrees_per_foot = 3.16540841682098E-06;}
if(POBlat >= 30.2 && POBlat < 30.4){x_long_degrees_per_foot = 3.17183411307589E-06;}
if(POBlat >= 30.4 && POBlat < 30.6){x_long_degrees_per_foot = 3.17833645869752E-06;}
if(POBlat >= 30.6 && POBlat < 30.8){x_long_degrees_per_foot = 3.18489594944933E-06;}
if(POBlat >= 30.8 && POBlat < 31){x_long_degrees_per_foot = 3.19152331407781E-06;}
if(POBlat >= 31 && POBlat < 31.2){x_long_degrees_per_foot = 3.19821923153188E-06;}
if(POBlat >= 31.2 && POBlat < 31.4){x_long_degrees_per_foot = 3.20497411983398E-06;}
if(POBlat >= 31.4 && POBlat < 31.6){x_long_degrees_per_foot = 0.000003211809179993;}
if(POBlat >= 31.6 && POBlat < 31.8){x_long_degrees_per_foot = 3.21870453579843E-06;}
if(POBlat >= 31.8 && POBlat < 32){x_long_degrees_per_foot = 3.2256711815311E-06;}
if(POBlat >= 32 && POBlat < 32.2){x_long_degrees_per_foot = 0.00000323270985136;}
if(POBlat >= 32.2 && POBlat < 32.4){x_long_degrees_per_foot = 3.23981079504957E-06;}
if(POBlat >= 32.4 && POBlat < 32.6){x_long_degrees_per_foot = 3.24699571721265E-06;}
if(POBlat >= 32.6 && POBlat < 32.8){x_long_degrees_per_foot = 3.25424434819113E-06;}
if(POBlat >= 32.8 && POBlat < 33){x_long_degrees_per_foot = 3.26156796618406E-06;}
if(POBlat >= 33 && POBlat < 33.2){x_long_degrees_per_foot = 3.26895667978608E-06;}
if(POBlat >= 33.2 && POBlat < 33.4){x_long_degrees_per_foot = 3.27643262016317E-06;}
if(POBlat >= 33.4 && POBlat < 33.6){x_long_degrees_per_foot = 3.28397518628349E-06;}
if(POBlat >= 33.6 && POBlat < 33.8){x_long_degrees_per_foot = 3.29159589735487E-06;}
if(POBlat >= 33.8 && POBlat < 34){x_long_degrees_per_foot = 3.29928471507377E-06;}
if(POBlat >= 34 && POBlat < 34.2){x_long_degrees_per_foot = 3.3070532832425E-06;}
if(POBlat >= 34.2 && POBlat < 34.4){x_long_degrees_per_foot = 3.31490247556917E-06;}
if(POBlat >= 34.4 && POBlat < 34.6){x_long_degrees_per_foot = 3.32283318048301E-06;}
if(POBlat >= 34.6 && POBlat < 34.8){x_long_degrees_per_foot = 3.33083520692814E-06;}
if(POBlat >= 34.8 && POBlat < 35){x_long_degrees_per_foot = 3.3389204602368E-06;}
if(POBlat >= 35 && POBlat < 35.2){x_long_degrees_per_foot = 3.34707866973705E-06;}
if(POBlat >= 35.2 && POBlat < 35.4){x_long_degrees_per_foot = 3.35533313425023E-06;}
if(POBlat >= 35.4 && POBlat < 35.6){x_long_degrees_per_foot = 3.36366235557275E-06;}
if(POBlat >= 35.6 && POBlat < 35.8){x_long_degrees_per_foot = 3.37206714460098E-06;}
if(POBlat >= 35.8 && POBlat < 36){x_long_degrees_per_foot = 3.38055975308392E-06;}
if(POBlat >= 36 && POBlat < 36.2){x_long_degrees_per_foot = 3.38914119162204E-06;}
if(POBlat >= 36.2 && POBlat < 36.4){x_long_degrees_per_foot = 3.39780094322954E-06;}
if(POBlat >= 36.4 && POBlat < 36.6){x_long_degrees_per_foot = 3.40655147980596E-06;}
if(POBlat >= 36.6 && POBlat < 36.8){x_long_degrees_per_foot = 3.41539386322031E-06;}
if(POBlat >= 36.8 && POBlat < 37){x_long_degrees_per_foot = 3.42431744792469E-06;}
if(POBlat >= 37 && POBlat < 37.2){x_long_degrees_per_foot = 3.43332314780799E-06;}
if(POBlat >= 37.2 && POBlat < 37.4){x_long_degrees_per_foot = 3.44243559203007E-06;}
if(POBlat >= 37.4 && POBlat < 37.6){x_long_degrees_per_foot = 3.45162036317949E-06;}
if(POBlat >= 37.6 && POBlat < 37.8){x_long_degrees_per_foot = 3.46091416586777E-06;}
if(POBlat >= 37.8 && POBlat < 38){x_long_degrees_per_foot = 3.47028223805442E-06;}
if(POBlat >= 38 && POBlat < 38.2){x_long_degrees_per_foot = 3.47976170591838E-06;}
if(POBlat >= 38.2 && POBlat < 38.4){x_long_degrees_per_foot = 3.48932962999149E-06;}
if(POBlat >= 38.4 && POBlat < 38.6){x_long_degrees_per_foot = 3.49898704325098E-06;}
if(POBlat >= 38.6 && POBlat < 38.8){x_long_degrees_per_foot = 3.50874730703644E-06;}
if(POBlat >= 38.8 && POBlat < 39){x_long_degrees_per_foot = 3.51859931598429E-06;}
if(POBlat >= 39 && POBlat < 39.2){x_long_degrees_per_foot = 3.52855660863367E-06;}
if(POBlat >= 39.2 && POBlat < 39.4){x_long_degrees_per_foot = 3.53860798239189E-06;}
if(POBlat >= 39.4 && POBlat < 39.6){x_long_degrees_per_foot = 3.54876715828921E-06;}
if(POBlat >= 39.6 && POBlat < 39.8){x_long_degrees_per_foot = 3.55902283469051E-06;}
if(POBlat >= 39.8 && POBlat < 40){x_long_degrees_per_foot = 3.56938892061679E-06;}
if(POBlat >= 40 && POBlat < 40.2){x_long_degrees_per_foot = 3.57985401355333E-06;}
if(POBlat >= 40.2 && POBlat < 40.4){x_long_degrees_per_foot = 3.59043221623019E-06;}
if(POBlat >= 40.4 && POBlat < 40.6){x_long_degrees_per_foot = 3.60109905543172E-06;}
if(POBlat >= 40.6 && POBlat < 40.8){x_long_degrees_per_foot = 3.61189469159837E-06;}
if(POBlat >= 40.8 && POBlat < 41){x_long_degrees_per_foot = 3.62278149917944E-06;}
if(POBlat >= 41 && POBlat < 41.2){x_long_degrees_per_foot = 3.63378695107106E-06;}
if(POBlat >= 41.2 && POBlat < 41.4){x_long_degrees_per_foot = 3.6449126132201E-06;}
if(POBlat >= 41.4 && POBlat < 41.6){x_long_degrees_per_foot = 3.65614671385533E-06;}
if(POBlat >= 41.6 && POBlat < 41.8){x_long_degrees_per_foot = 3.66749062956144E-06;}
if(POBlat >= 41.8 && POBlat < 42){x_long_degrees_per_foot = 3.67894576130264E-06;}
if(POBlat >= 42 && POBlat < 42.2){x_long_degrees_per_foot = 3.69052715489881E-06;}
if(POBlat >= 42.2 && POBlat < 42.4){x_long_degrees_per_foot = 3.70222281457787E-06;}
if(POBlat >= 42.4 && POBlat < 42.6){x_long_degrees_per_foot = 3.71404801521274E-06;}
if(POBlat >= 42.6 && POBlat < 42.8){x_long_degrees_per_foot = 3.72597676480889E-06;}
if(POBlat >= 42.8 && POBlat < 43){x_long_degrees_per_foot = 3.73805225049436E-06;}
if(POBlat >= 43 && POBlat < 43.2){x_long_degrees_per_foot = 3.75023438964935E-06;}
if(POBlat >= 43.2 && POBlat < 43.4){x_long_degrees_per_foot = 3.76253866008473E-06;}
if(POBlat >= 43.4 && POBlat < 43.6){x_long_degrees_per_foot = 3.77498093634627E-06;}
if(POBlat >= 43.6 && POBlat < 43.8){x_long_degrees_per_foot = 3.78754881203532E-06;}
if(POBlat >= 43.8 && POBlat < 44){x_long_degrees_per_foot = 3.80025841757239E-06;}
if(POBlat >= 44 && POBlat < 44.2){x_long_degrees_per_foot = 3.81308268669806E-06;}
if(POBlat >= 44.2 && POBlat < 44.4){x_long_degrees_per_foot = 3.82605235570044E-06;}
if(POBlat >= 44.4 && POBlat < 44.6){x_long_degrees_per_foot = 3.83915477168547E-06;}
if(POBlat >= 44.6 && POBlat < 44.8){x_long_degrees_per_foot = 3.8523917574226E-06;}
if(POBlat >= 44.8 && POBlat < 45){x_long_degrees_per_foot = 3.86576517022897E-06;}
if(POBlat >= 45 && POBlat < 45.2){x_long_degrees_per_foot = 3.87929195163299E-06;}
if(POBlat >= 45.2 && POBlat < 45.4){x_long_degrees_per_foot = 3.89295919400173E-06;}
if(POBlat >= 45.4 && POBlat < 45.6){x_long_degrees_per_foot = 3.90675360495689E-06;}
if(POBlat >= 45.6 && POBlat < 45.8){x_long_degrees_per_foot = 3.92070760930933E-06;}
if(POBlat >= 45.8 && POBlat < 46){x_long_degrees_per_foot = 3.93482358218469E-06;}
if(POBlat >= 46 && POBlat < 46.2){x_long_degrees_per_foot = 3.94907275771649E-06;}
if(POBlat >= 46.2 && POBlat < 46.4){x_long_degrees_per_foot = 3.96347263618492E-06;}
if(POBlat >= 46.4 && POBlat < 46.6){x_long_degrees_per_foot = 3.97804121250696E-06;}
if(POBlat >= 46.6 && POBlat < 46.8){x_long_degrees_per_foot = 3.9927491675118E-06;}
if(POBlat >= 46.8 && POBlat < 47){x_long_degrees_per_foot = 4.00763052852631E-06;}
if(POBlat >= 47 && POBlat < 47.2){x_long_degrees_per_foot = 4.02265559631847E-06;}
if(POBlat >= 47.2 && POBlat < 47.4){x_long_degrees_per_foot = 4.03785896566205E-06;}
if(POBlat >= 47.4 && POBlat < 47.6){x_long_degrees_per_foot = 4.05322697665747E-06;}
if(POBlat >= 47.6 && POBlat < 47.8){x_long_degrees_per_foot = 4.06874552437992E-06;}
if(POBlat >= 47.8 && POBlat < 48){x_long_degrees_per_foot = 4.08445009006212E-06;}
if(POBlat >= 48 && POBlat < 48.2){x_long_degrees_per_foot = 4.10032679604564E-06;}
if(POBlat >= 48.2 && POBlat < 48.4){x_long_degrees_per_foot = 4.11637824576425E-06;}
if(POBlat >= 48.4 && POBlat < 48.6){x_long_degrees_per_foot = 4.13260709651291E-06;}
if(POBlat >= 48.6 && POBlat < 48.8){x_long_degrees_per_foot = 4.14899884657832E-06;}
if(POBlat >= 48.8 && POBlat < 49){x_long_degrees_per_foot = 4.16559055577309E-06;}
if(POBlat >= 49 && POBlat < 49.2){x_long_degrees_per_foot = 4.18236797309901E-06;}
if(POBlat >= 49.2 && POBlat < 49.4){x_long_degrees_per_foot = 4.19931635129801E-06;}
if(POBlat >= 49.4 && POBlat < 49.6){x_long_degrees_per_foot = 4.21647376299201E-06;}
if(POBlat >= 49.6 && POBlat < 49.8){x_long_degrees_per_foot = 4.23380780206102E-06;}
if(POBlat >= 49.8 && POBlat < 50){x_long_degrees_per_foot = 4.25380780206102E-06;}

return x_long_degrees_per_foot;

}

double get_lat_ratio(double POBlat)

{

//Notice how both the lat and long appear to be more sensitive to North/South moves than east west
double Y_lat_degrees_per_foot = 0;

if(POBlat >= 25 && POBlat < 26){Y_lat_degrees_per_foot = 2.75142523827343E-06;}
if(POBlat >= 26 && POBlat < 27){Y_lat_degrees_per_foot = 2.75104677329724E-06;}
if(POBlat >= 27 && POBlat < 28){Y_lat_degrees_per_foot = 2.75065328015404E-06;}
if(POBlat >= 28 && POBlat < 29){Y_lat_degrees_per_foot = 2.7502523356518E-06;}
if(POBlat >= 29 && POBlat < 30){Y_lat_degrees_per_foot = 2.74984394635604E-06;}
if(POBlat >= 30 && POBlat < 31){Y_lat_degrees_per_foot = 2.74942811895126E-06;}
if(POBlat >= 31 && POBlat < 32){Y_lat_degrees_per_foot = 2.74899730323365E-06;}
if(POBlat >= 32 && POBlat < 33){Y_lat_degrees_per_foot = 2.74856662250636E-06;}
if(POBlat >= 33 && POBlat < 34){Y_lat_degrees_per_foot = 2.74812852447483E-06;}
if(POBlat >= 34 && POBlat < 35){Y_lat_degrees_per_foot = 2.74767546655529E-06;}
if(POBlat >= 35 && POBlat < 36){Y_lat_degrees_per_foot = 2.74722255799387E-06;}
if(POBlat >= 36 && POBlat < 37){Y_lat_degrees_per_foot = 2.74676225399311E-06;}
if(POBlat >= 37 && POBlat < 38){Y_lat_degrees_per_foot = 2.74630210421667E-06;}
if(POBlat >= 38 && POBlat < 39){Y_lat_degrees_per_foot = 2.74583456895889E-06;}
if(POBlat >= 39 && POBlat < 40){Y_lat_degrees_per_foot = 2.74535965584171E-06;}
if(POBlat >= 40 && POBlat < 41){Y_lat_degrees_per_foot = 2.74488490697585E-06;}
if(POBlat >= 41 && POBlat < 42){Y_lat_degrees_per_foot = 2.7444103222761E-06;}
if(POBlat >= 42 && POBlat < 43){Y_lat_degrees_per_foot = 2.74392837249376E-06;}
if(POBlat >= 43 && POBlat < 44){Y_lat_degrees_per_foot = 2.74344659195347E-06;}
if(POBlat >= 44 && POBlat < 45){Y_lat_degrees_per_foot = 2.74296498056609E-06;}
if(POBlat >= 45 && POBlat < 46){Y_lat_degrees_per_foot = 2.74248353824256E-06;}
if(POBlat >= 46 && POBlat < 47){Y_lat_degrees_per_foot = 2.74200226489387E-06;}
if(POBlat >= 47 && POBlat < 48){Y_lat_degrees_per_foot = 2.74152116043108E-06;}
if(POBlat >= 48 && POBlat < 49){Y_lat_degrees_per_foot = 2.7410402247653E-06;}
if(POBlat >= 49 && POBlat < 50){Y_lat_degrees_per_foot = 2.74056696849444E-06;}

return Y_lat_degrees_per_foot;
}

bool authenticate_license()
{
	return 0;
	/*
	long dan = time(0);
                if(dan > 2483228800)  //This expiration date is at precisely midnight on 12/31/2016.
				{
					return 0;
                }
                else
				{
					return 1;
                }*/
}
