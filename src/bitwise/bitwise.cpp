void check_all_crcs(RESULTS_STR CRC_bool[NUM_SITES])
{
	int byte=0;
	int site;
	int stored_index = 0;
  	int CRC_POLY=0x7;
	groupgetresults(CRC_bool, NUM_SITES);
	
	INT64	captured_data[3072];

	bool crc_ok[NUM_SITES] = {true, true};

	int bit, starting_bit, hex_line;

	int my_crc;
	int ee_crc;
	
	FOR_EACH_SITE(site, NUM_SITES)
	{
		dpingetcapturedata( "SDA", DPIN_CAPTDATA, 3072, captured_data, site ); < ETS  ignore

		my_crc = 0;
		ee_crc = 0;
		starting_bit = 0;

		for(hex_line=0; hex_line<12; hex_line++)
		{
			// Skip the ECC lines, which no longer have a valid 30-byte CRC attached
			if(hex_line == 2 || hex_line == 9 || hex_line == 10)
			{
				starting_bit += 256;
				continue;
			}

			for(bit=starting_bit; bit<starting_bit+256; bit++)
			{
				if(bit%8==0&&bit!=starting_bit)
				{
					my_crc ^= byte;
					for(int i=0; i<8; i++)
					{
						if(my_crc & 0x80)
							my_crc = (my_crc<<1)^CRC_POLY;
						else
							my_crc <<= 1;
					}
		
					byte = 0;
				}
				
				byte <<= 1;
				byte += (int)captured_data[bit];
			}

			ee_crc = byte;

			my_crc &= 0x000000FF;

			if(my_crc != ee_crc)
				crc_ok[site] = false;

			my_crc = 0;

			starting_bit += 256;
		}

		if(crc_ok[site])
			CRC_bool[site].value = 0;
		else
			CRC_bool[site].value = 1;
	}
	
	return;
}




int htoi_crc(const char *ptr)
{
	int value = 0;
	char ch = *ptr;
	
	for (;;) 
	{

	    if (ch >= '0' && ch <= '9')
	        value = (value << 4) + (ch - '0');
	    else if (ch >= 'A' && ch <= 'F')
	        value = (value << 4) + (ch - 'A' + 10);
	    else if (ch >= 'a' && ch <= 'f')
	        value = (value << 4) + (ch - 'a' + 10);
	    else
	       return value;
	   ch = *(++ptr);
	}
}

int htoi_string(string hex_number)
{
	int value = 0;
	char ch;
	unsigned i = 2;
	
	while(i < hex_number.length()){
		ch = hex_number.at(i);
	    if (ch >= '0' && ch <= '9')
	        value = (value << 4) + (ch - '0');
	    else if (ch >= 'A' && ch <= 'F')
	        value = (value << 4) + (ch - 'A' + 10);
	    else if (ch >= 'a' && ch <= 'f')
	        value = (value << 4) + (ch - 'a' + 10);
	    	       
		i++;
	}

	return value;
}

int htoi(string hex_number)
{
	int value = 0;
	char ch;
	unsigned i = 0;
	
	while(i < hex_number.length()){
		ch = hex_number.at(i);
	    if (ch >= '0' && ch <= '9')
	        value = (value << 4) + (ch - '0');
	    else if (ch >= 'A' && ch <= 'F')
	        value = (value << 4) + (ch - 'A' + 10);
	    else if (ch >= 'a' && ch <= 'f')
	        value = (value << 4) + (ch - 'a' + 10);
	    	       
		i++;
	}

	return value;
}

vector<string> &split(const string &s, char delim, vector<std::string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, ',')) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

string address_to_string(int address)
{
	string output = "0x";

	int nibble;
	int mask = 0xF00;
	int shift = 8;

	for(int i=0; i<3; i++)
	{
		nibble = (address&mask);
		nibble >>= shift;
		
		switch (nibble)
		{
			case 0:{output += "0";break;}
			case 1:{output += "1";break;}
			case 2:{output += "2";break;}
			case 3:{output += "3";break;}
			case 4:{output += "4";break;}
			case 5:{output += "5";break;}
			case 6:{output += "6";break;}
			case 7:{output += "7";break;}
		    case 8:{output += "8";break;}
			case 9:{output += "9";break;}
			case 10:{output += "A";break;}
			case 11:{output += "B";break;}
			case 12:{output += "C";break;}
			case 13:{output += "D";break;}
			case 14:{output += "E";break;}
			case 15:{output += "F";break;}
		}

		mask >>= 4;
		shift -= 4;
	}

	return output;
}