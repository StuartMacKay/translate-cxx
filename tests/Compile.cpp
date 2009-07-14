/*
 *  Compile.cpp
 *  Translate AS1
 *
 *  Created by smackay on Tue Jun 15 2004.
 *  Copyright (c) 2004 Flagstone Software Ltd. All rights reserved.
 *
 *  This file is part of the Translate AS1 library. You may not use this file except in
 *  compliance with the terms of the license (the 'License') that accompanied this file.
 *
 *  The Original Code and all software distributed under the License are distributed on an 
 *  'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND Flagstone 
 *  HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF 
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT OF THIRD PARTY 
 *  RIGHTS. Please see the License for the specific language governing rights and limitations 
 *  under the License.
 */

#include "UnitTest.hpp"

#include <fstream>

class Compile : public UnitTest
{
public:
    Compile(int argc, const char * argv[]) : UnitTest(argc, argv)
    {
	    vector<string> files;
    
        string dirIn = stringOption(options, "dirIn");
        string dirOut = stringOption(options, "dirOut");
        string extension = "as";

        string fileIn = "";
        string fileOut = "";
        string script = "";
    
        findFiles(dirIn, files, extension);
	        
		for (vector<string>::iterator i = files.begin(); i != files.end(); ++i)
		{
			fileIn = *i;
			fileOut = fileIn.substr(0, fileIn.find_first_of('.')) + ".swf";

			cout << fileIn << endl;

	#ifdef WIN32
			script = "#include \"" + fileIn + "\"\n";
			fileOut = dirOut + "\\" + fileOut;
	#else
			script = "#include \"" + fileIn + "\"\n";
			fileOut = dirOut + "/" + fileOut;
	#endif

			ASParser* parser = Parser();
			ASNode* root = parser->parse(script.c_str());

	//        ostringstream buffer(ostringstream::out);
	//        show(root, buffer, "");
	//        cout << buffer.str().c_str();

			if (root != 0)
			{
				int swfVersion = 5;

				fstream out;
	       
				size_t size = parser->compile(root, swfVersion);

				unsigned char* encodedActions = new unsigned char[size];
				parser->getCode(encodedActions);

				out.open(fileOut.c_str(), fstream::out | fstream::binary);
				unsigned char signature[] = { 0x46, 0x57, 0x53, 0x05 };

				for (int i=0; i<4; i++)
					out << signature[i];

				if (root->getType() == ASNode::Array)
				{
					size_t fileLength = 35 + size + 1;
					size_t actionLength = size + 1;

					for (unsigned int i=0; i<4; i++, fileLength >>= 8)
						out << (unsigned char)fileLength;
	        			
					unsigned char header[] = { 
						0x68, 0x00, 0x1f, 0x40, 0x00, 0x07, 0xd0, 0x00,
						0x00, 0x0c, 0x01, 0x00, 0x43, 0x02, 0xff, 0xff,
						0xff, 0x3f, 0x03
					};
	        				
					for (unsigned int i=0; i<19; i++)
						out << header[i];
	        					
					for (unsigned int i=0; i<4; i++, actionLength >>= 8)
						out << (unsigned char)actionLength;

					for (unsigned int i=0; i<size; i++)
						out << (unsigned char)encodedActions[i];

					out << (unsigned char)0;
				}
				else if (root->getType() == ASNode::Button)
				{
					size_t fileLength = 131 + size + 1;
					size_t actionLength = size + 13;

					for (unsigned int i=0; i<4; i++, fileLength >>= 8)
						out << (unsigned char)fileLength;
	        			
					unsigned char header[] = { 
						0x78, 0x00, 0x05, 0x5f, 0x00, 0x00, 0x0f, 0xa0, 
						0x00, 0x00, 0x0c, 0x01, 0x00, 0x43, 0x02, 0xff, 
						0xff, 0xff, 0xbf, 0x00, 0x4d, 0x00, 0x00, 0x00,
						0x01, 0x00, 0x64, 0x63, 0xb4, 0xcd, 0x21, 0x2e, 
						0x90, 0x01, 0x00, 0x00, 0xcc, 0x66, 0x01, 0x14, 
						0x00, 0x00, 0x33, 0x66, 0x11, 0x35, 0x89, 0x18, 
						0x83, 0x3d, 0x37, 0xf4, 0xd7, 0xd2, 0x70, 0x01, 
						0x4d, 0x27, 0x00, 0x17, 0xf7, 0x28, 0xd3, 0x7f, 
						0x72, 0x90, 0x00, 0xb3, 0xf4, 0x80, 0x0b, 0x3e, 
						0x80, 0xf2, 0x8d, 0x28, 0x0f, 0x29, 0x2d, 0x90, 
						0x01, 0x4a, 0xd9, 0x00, 0x08, 0x0c, 0xd7, 0x52,
						0x80, 0xcd, 0x78, 0x00, 0x4c, 0x34, 0x80, 0x04, 
						0xc3, 0x7f, 0x4d, 0x70, 0x00, 0xbf, 0x08
					};

					for (unsigned int i=0; i<103; i++)
						out << header[i];
	        					
					for (unsigned int i=0; i<4; i++, actionLength >>= 8)
						out << (unsigned char)actionLength;

					unsigned char body[] = { 
						0x01, 0x00, 0x00, 0x08, 0x00, 0x01, 0x01, 0x00,
						0x01, 0x00, 0x00, 0x00, 0x00
					};
	        		
					for (unsigned int i=0; i<13; i++)
						out << (unsigned char)body[i];

					for (unsigned int i=0; i<size; i++)
						out << (unsigned char)encodedActions[i];

					unsigned char tail[] = {
						0x8a, 0x06, 0x06, 0x01, 0x00, 0x02, 0x00, 0x1c, 
						0xad, 0x31, 0xd7, 0x80
					};
	            	
					for (unsigned int i=0; i<12; i++)
						out << (unsigned char)tail[i];
				}
				else if (root->getType() == ASNode::MovieClip)
				{
					size_t fileLength = 108 + size;
					size_t actionLength = size + 9;

					for (unsigned int i=0; i<4; i++, fileLength >>= 8)
						out << (unsigned char)fileLength;
	        			
					unsigned char header[] = { 
						0x68, 0x00, 0x1f, 0x40, 0x00, 0x07, 0xd0, 0x00,
						0x00, 0x0c, 0x01, 0x00, 0x43, 0x02, 0xff, 0xff,
						0xff, 0xbf, 0x00, 0x24, 0x00, 0x00, 0x00, 0x01, 
						0x00, 0x5d, 0x93, 0x4d, 0xba, 0xd0, 0xa6, 0x80, 
						0x01, 0x00, 0xff, 0xff, 0xff, 0x01, 0x14, 0x00, 
						0x00, 0x00, 0x00, 0x11, 0x35, 0x69, 0x8c, 0xa1, 
						0xfd, 0x16, 0x75, 0xcb, 0x5e, 0xfa, 0x13, 0x1b, 
						0x95, 0x42, 0x80, 0xff, 0x09, 0x10, 0x00, 0x00, 
						0x00, 0x02, 0x00, 0x01, 0x00, 0x86, 0x06, 0x06, 
						0x01, 0x00, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 
						0x00, 0xbf, 0x06
					};

					for (unsigned int i=0; i<83; i++)
						out << header[i];
	        					
					for (unsigned int i=0; i<4; i++, actionLength >>= 8)
						out << (unsigned char)actionLength;

					unsigned char body[] = { 0x86, 0x01, 0x00, 0x02, 0x00, 0x18, 0xdf, 0x4a, 0xcc };

					for (unsigned int i=0; i<9; i++)
						out << (unsigned char)body[i];

					for (unsigned int i=0; i<size; i++)
						out << (unsigned char)encodedActions[i];
				}
				delete root;
				delete encodedActions;

				out << (unsigned char)64;
				out << (unsigned char)0;
				out << (unsigned char)0;
				out << (unsigned char)0;

				out.close();
			}
			else
			{
				string errorName;

				switch (parser->getError())
				{
				case ASParser::READ_ERROR: errorName = "An error occurred while reading the script."; break;
				case ASParser::PARSE_ERROR: errorName = "An error occurred while parsing the script."; break;
				}
	                
				cerr << errorName << endl;
				cerr << parser->getMessage() << endl;
				cerr << parser->getFilename() << ": Line " << parser->getLineNumber() << endl;
				cerr << parser->getLine() << endl;
			}
			delete parser;
		}
    }
};

int main (int argc, const char * argv[])
{
    Compile compiler(argc, argv);

    return 0;
}
