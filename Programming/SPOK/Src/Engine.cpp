﻿/*
Copyright 2017 Borja Gomez - kub0x@elhacker.net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../Headers/Engine.h"
#include "../Headers/FileHandler.h"
#include "../Headers/ParamHandler.h"
#include "../Headers/Crypto.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string.h>

Engine::Engine() : Singleton<Engine>(){
	i= 1, j= 5, nhash = 0, bfpos = 0, bufflimit = 0;
	totalstorage = 0;
	verbose = false;
	CHARSET = "abcdefghijklmnopqrstuvwxyz";
}

void Engine::ExecuteArgs(int argc, char **argv){
	int paramcount = 0;
	bool version = false;

	std::vector<std::string> args(argv, argv+argc); //split argv

	args.erase(args.begin()); //delete input filename

	std::string _charset;

	 //Check if no repetition, no ilegal commands and exist commands with arguments
	if (ParamHandler::GetInstance()->ParseArguments(args, &paramcount, &verbose, &version,
		dumpfile, loadfile, savefile, _charset, interval, hash, lastword)){
		if (version)
			if (paramcount == 1){
				std::cout << VERSION << std::endl;
				return;
			}else{
				PrintMenu();
				return; //version with more than 1 command considered illegal
			}
		
		if (!loadfile.empty()){
			if (paramcount == 1){
				std::string params  = FileHandler::GetInstance()->ReadStateFile(loadfile);
				if (params.empty()) return;
				//split params by whitespace
				args = Split(params);
				//Check if load file params are correct
				if (!ParamHandler::GetInstance()->ParseArguments(args, &paramcount, &verbose, &version,
					dumpfile, loadfile, savefile, _charset, interval, hash, lastword)){
					std::cout << "Failed to load state file!" << std::endl;
					return;
				}
			}else{
				PrintMenu(); //Load present but more than 1 command
				return;
			}
		}
		if (FillParams(_charset))
			BeginExecution();
		else
			PrintMenu();
	}else{
		PrintMenu(); //illegal cmds, repetition or no args for cmds
	}

}

std::vector<std::string> Engine::Split(const std::string& str){
	std::vector<std::string> v;
	std::stringstream strstream(str);
	std::string buff;
	while (strstream >> buff)
		v.push_back(buff);
	return v;
}

bool Engine::FillParams(const std::string& _charset){
	bool ret = true;

	if (!_charset.empty())
		CHARSET = _charset;
	else
		std::cout << "Using default charset." << std::endl;

	if (!interval.empty()){
		std::string concat;
		for (auto it=interval.begin(); it != interval.end(); it++){
			if (*it == ','){
				i = std::stoi(concat);
				concat.clear();
			}else{
				concat += *it;
			}
		}
		j = std::stoi(concat);
		if (i > j || j <= 0 || i <= 0)
			ret=false;
		
	}else{
		interval = "1,5";
	}

	if (!hash.empty()){
		nhash = hash.at(0) - '0';
		if (nhash < 0 || nhash > 3)
			ret = false;
	}else{
		hash = "0";
	}

	if (!savefile.empty()){
		if (verbose)
			saveparams = "-v ";
		saveparams += "-g " + dumpfile + " -c " + CHARSET + " -i " + interval + " -h " + hash + " -w ";
	}

	return ret;

}

void Engine::BeginExecution(){
	//Dont call CalcBuffLimit here (Save iterations)
	if (!nhash){
		for (int k = i; k <= j; k++){
			totalstorage += std::pow(CHARSET.size(), k)*(k+1);
			bufflimit += k+1;
		}
	}else{
		int m = 32;
		for (int k = i; k <= j; k++)
			totalstorage += std::pow(CHARSET.size(), k);
		if (nhash > 1)
			m+=40;
		if (nhash == 3)
			m+=64;
		m+=nhash;
		bufflimit = m;
		totalstorage *= m;
		std::cout << "Hashing with " << nhash << " algorithm(s)" << std::endl;
	}
	bufflimit = BUFFSIZE - (BUFFSIZE % bufflimit); 
	totalstorage /= (1024*1024);
	std::cout << "Total storage needed: " << totalstorage << " MB" << std::endl;
	int start = i;
	if (!dumpfile.empty()) FileHandler::GetInstance()->OpenDumpFile(dumpfile);
	FillNodeList();
	Permute();
	//WRITE REMAINING WORDS TO FILE OR PRINT LAST WORDS
	if (!dumpfile.empty())
	 	FileHandler::GetInstance()->LogFile(buffer, savefile, saveparams, bfpos);
	else
		PrintWords();
	if (!dumpfile.empty()) FileHandler::GetInstance()->CloseDumpFile();
	std::cout << "All words of length " << start << "," << j << " have been generated." << std::endl;
}

void Engine::CalculateBuffLimit(){
	//Avoid buffer overflow by calculating exact rounds of buffering
	if (i > j) return;
	bufflimit = 0;
	if (!nhash){
		for (int k = i; k <= j; k++)
			bufflimit += k+1;
	}else{
		bufflimit = 32;
		if (nhash > 1)
			bufflimit+=40;
		if (nhash == 3)
			bufflimit+=64;
		//Adds newline value per hash algorithm
		bufflimit+=nhash;
	}
	bufflimit = BUFFSIZE -  (BUFFSIZE % bufflimit);
}

void Engine::FillNodeList(){
	bool signaled = false;
	char value = CHARSET[0];
	int valuepos = 0;
	Node *node = nullptr, *prev = nullptr;

	for (int k=0; k < j; k++){
		if (lastword.size()){
			value = lastword[k];
			valuepos = GetLetterPos(value);
		}
		node = new Node(value, CHARSET[0], valuepos, signaled, prev);
		nodes.push_back(node);
		prev = node;
		if (value != CHARSET[0])
			signaled = true;
	}

	if (!lastword.size()) //AVOIDS REPEATING LAST WORD
		GenerateWords(); //WRITE FIRST WORD(S)

}

int Engine::GetLetterPos(char c){
	int pos = 0, ctr = 0;
	while (ctr < CHARSET.size() && !pos){
		if (CHARSET[ctr] == c)
			pos = ctr;
		ctr++;
	}
	return pos;
}

void Engine::Permute(){
	int seqcount = 0, valuepos = 0;
	Node *node = nodes.back();
	start = std::chrono::steady_clock::now();
	while(node != nullptr){
		valuepos = node->getValuePos();
		if (valuepos < CHARSET.size()-1){
			node->Permute(CHARSET.at(valuepos + 1));
			node = nodes.back();
			if (verbose)
				words++;
			
			GenerateWords();
		}else{
			if (!node->IsSignaled())
				//SAVE FUTURE ITERATIONS ON SUBSTR
				if (++seqcount == i){ 
					i++;
					//Re-calculate buffer limit to avoid overflow
					CalculateBuffLimit();
				}
			node->Reset();
			node = node->Prev();
		}
	}
}

void Engine::ShowVerbose(){
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> ms = end - start;
	if (ms.count() > 60){
		long double curstorage = FileHandler::GetInstance()->GetFileSize(dumpfile);
		std::cout << "Words per minute: " << words << "\nWords per second: " << words/60 << std::endl;
		std::cout << "Current storage: " << curstorage/(1024*1024) << "/" << totalstorage << " MB" << std::endl;
		start = std::chrono::steady_clock::now();
		words = 0;
	}
}

void Engine::SubStrCrypto(){
	int start = 0;
	int offset = j;
	for (int k = i; k <= j; k++){
		char word[offset+1];
		int p = 0;
		for (int k = start; k < j; k++){
			word[p] = nodes.at(k)->getValue();
			p++;
		}
		Crypto::GetInstance()->HashWord(buffer, word, nhash, &bfpos, offset);
		offset--;
		start++;
	}
}


void Engine::SubStrWord(){
	int start =0;
	int p = 0;
	int offset = j;
	for (int h = i; h <= j; h++){
		for (int k = bfpos; k < bfpos + offset; k++){
			buffer[k] = nodes.at(p)->getValue();
			p++;
		}
		bfpos += offset;
		buffer[bfpos++] = '\n';
		start++;
		p=start;
		offset--;
	}
}

void Engine::GenerateWords(){
	//AVOID CHECKING HASH FLAG EVERY ITERATION - SPLIT INTO 2 FOR
	if (!nhash)
		SubStrWord();
	else
		SubStrCrypto();

	if (bfpos >= bufflimit){
		if (verbose)
			ShowVerbose();
		
		if (!dumpfile.empty())
			FileHandler::GetInstance()->LogFile(buffer, savefile, saveparams, bfpos);
		else
			PrintWords();
		
		memset(buffer,0,bfpos);
		bfpos = 0;
	}
}

void Engine::PrintWords(){
	printf("%s", buffer);
}

void Engine::PrintMenu(){
	std::cout << "\n	S.P.O.K (Simple Production Of Keys) Copyright (C) 2017 Borja Gomez - kub0x@elhacker.net\n" << std::endl;
	std::cout << "usage: spok [options] command [ARGUMENTS]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "	-ver, --version	(show version)" << std::endl;
	std::cout << "	-v, --verbose	(show advanced console log)" << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "	-g [FILE], --generate [FILE]		(generate dictionary in specified FILE)" << std::endl;
	std::cout << "	-l [SAVEFILE], --load [SAVEFILE]	(load previous session state)" << std::endl; 
	std::cout << "Arguments:" << std::endl;
	std::cout << "	-c [CHARSET], --charset [CHARSET]	(use specified charset, default is: 'abcdefghijklmnopqrstuwvxyz')" << std::endl;
	std::cout << "	-i <min,max>, --interval <min,max>	(generate all words of length min & max, default is: <0,5>)" <<std::endl;
	std::cout << "	-h <number>, --hash <number>		(hash output words into FILE, algorithms: MD5, SHA-1, SHA-256)" << std::endl;
	std::cout << "	-s [SAVEFILE], --save [SAVEFILE]	(save current session state in SAVEFILE)" << std::endl;
	std::cout << "\n Example: spok --generate words.txt --save save.sav --interval 4,8 --charset '01234567890ABCDEF' --hash 3" << std::endl;
}