#ifndef BITREADERWRITER_H
#define BITREADERWRITER_H

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

namespace BIT_MANIPULATOR
{


	
	//HERE in a byte: bits are in the order [7..0] the 0th index is the bit on the full right, the 7th bit is on the full left

	typedef unsigned char LByte;
	typedef int LBit;

	
class BitReader{
public:
	LByte* data;
	int dataSize;
	int bitPointer;
	int bytePointer;
	
	BitReader(const char* fname)
	{
		data = NULL;
		dataSize = 0;
		resetPointers();
		readFile(fname);
	}

	BitReader(std::string fname)
	{
		data = NULL;
		dataSize = 0;
		resetPointers();
		readFile(fname.c_str());
	}

	void error(char* err_name)
	{
		std::cout << err_name << std::endl;
		system("PAUSE");
		exit(1);
	}
	~BitReader()
	{
		if(data != NULL)
		{
			delete [] data;
		}
	}
	void readFile(const char* fname)
	{
		FILE* fi = fopen(fname, "rb");
		if(fi == NULL)
		{
			error("error in BitReader.readFile(), FILE COULD NOT BE READ");
		}
		std::vector<LByte> list;
		while(1)
		{
			unsigned char buffer[51];
			int numRead = fread(buffer, sizeof(unsigned char), 50, fi);
			for(int i = 0; i < numRead; i++)
			{
				list.push_back(buffer[i]);
			}
			if(feof(fi))
			{
				break;
			}
	
		}
		fclose(fi);
		dataSize = list.size();
		data = new LByte[dataSize];
		for(int i = 0; i < dataSize; i++)
		{
			data[i] = list.at(i);
		}
	}
	void resetPointers()
	{
		bitPointer = 0;
		bytePointer = 0;
	}
	int pointersAtEnd()
	{
		if(bytePointer == dataSize)
		{
			return 1;
		}
		return 0;
	}
	LBit readBit()
	{
		if(bytePointer == dataSize)
		{
			std::cout << "Reached End of File\n";
			return 0;
		}
		LBit rv = getBit(data[bytePointer], bitPointer);
		bitPointer++;
		if(bitPointer == 8)
		{
			bitPointer = 0;
			bytePointer++;
		}
		return rv;
	}
	LByte readByte()
	{
		LByte rv = 0x00;
		for(int i = 0; i < 8; i++)
		{
			setBit(rv, i, readBit());
		}
		return rv;
	}
	void readBytes(int num, LByte* bytes)
	{
		for(int i = 0; i < num; i++)
		{
			bytes[i] = readByte();
		}
	}
	LBit getBit(LByte in, int at)
	{
		//at = 7 - at;
		if(at < 0 || at > 7)
		{ 
			error("ERROR - in BitReader.getBit(), index out of range");
			return 0;
		}
		in >>= at;
		in = in & 0xFF;
		if(in & 0x01){
			return 1;
		}else{
			return 0;
		}
	}
	void setBit(LByte& input, int location, LBit value)
	{
		if(location < 0 || location > 7)
		{
			error("ERROR - BitReader.setBit(), index not correct");
		}

		LByte mask[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
		if(value == 1)
		{
			input = input | mask[location];
		}
		else
		{
			input = input & (~mask[location]);
		}
	}
	void print(LByte byte)
	{
		for(int i = 0; i < 8; i++)
		{
			std::cout << getBit(byte, i);
		}
		std::cout << std::endl;
	}
	void print(LByte byte, int index)
	{
		std::cout << getBit(byte, index) << std::endl;
	}
	void print(int num)	//prints all bytes read by this construct up to num
	{
		int savedBitPointer = bitPointer;
		int savedBytePointer = bytePointer;
		resetPointers();
		for(int i = 0; i < num && i < dataSize; i++)
		{
			LByte byte = readByte();
			print(byte);
		}
		bitPointer = savedBitPointer;
		bytePointer = savedBytePointer;
	}
	void saveRaw(char* fname)
	{
		int savedBitPointer = bitPointer;
		int savedBytePointer = bytePointer;
		resetPointers();
		FILE* fi = fopen(fname, "wb");
		for(int i = 0; i < dataSize; i++)
		{
			LByte byte = readByte();
			fwrite(&byte, sizeof(unsigned char), 1, fi);
		}
		fclose(fi);
		bitPointer = savedBitPointer;
		bytePointer = savedBytePointer;
	}
	void flipBits(LByte& c)
	{
		for(int i = 0; i < 8; i++)
		{
			if(getBit(c, i) == 1)
			{
				setBit(c, i, 0);
			}
			else
			{
				setBit(c, i, 1);
			}
		}
	}

};



	class BitWriter{
	LByte work_on;
public:
	int dataSize;
	int bitPointer;
	std::vector<LByte>* data;	
	BitWriter()
	{
		work_on = 0x00;
		bitPointer = 0;
		dataSize = 0;
		data = new std::vector<LByte>;
	}
	~BitWriter()
	{
		delete data;
	}
	void error(char* err_name)
	{
		std::cout << err_name << std::endl;
		system("PAUSE");
		exit(1);
	}
	LBit getBit(LByte in, int at)
	{
		//at = 7 - at;
		if(at < 0 || at > 7)
		{ 
			error("ERROR - in BitWriter.getBit(), index out of range");
			return 0;
		}
		in >>= at;
		in = in & 0xFF;
		if(in & 0x01){
			return 1;
		}else{
			return 0;
		}
	}
	void setBit(LByte& input, int location, LBit value)
	{
		if(location < 0 || location > 7)
		{
			error("ERROR - BitWriter.setBit(), index not correct");
		}

		LByte mask[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
		if(value == 1)
		{
			input = input | mask[location];
		}
		else
		{
			input = input & (~mask[location]);
		}
	}
	void addBit(LBit input)
	{
		setBit(work_on, bitPointer, input);
		bitPointer++;
		if(bitPointer >= 8)
		{
			data->push_back(work_on);
			work_on = 0x00;
			bitPointer = 0;
			dataSize++;
		}

	}
	void addByte(LByte input)
	{
		for(int i = 0; i < 8; i++)
		{
			addBit(getBit(input, i));
		}
	}
	void addBytes(int num, unsigned char* bytes)
	{
		for(int i = 0; i < num; i++)
		{
			addByte(bytes[i]);
		}
	}
	
	void operator+=(LBit bit_in)
	{
		addBit(bit_in);
	}

	void operator+=(LByte byte_in)
	{
		addByte(byte_in);
	}

	void save(char* fname)
	{
		FILE* fi = fopen(fname, "wb");
		for(int i = 0; i < dataSize; i++)
		{
			LByte byte = data->at(i);
			fwrite(&byte, 1, 1, fi);
		}
		if(bitPointer > 0)
		{
			fwrite(&work_on, 1, 1, fi);
		}
		fclose(fi);
	}

	void save(const char * fname)
	{
		FILE* fi = fopen(fname, "wb");
		for(int i = 0; i < dataSize; i++)
		{
			LByte byte = data->at(i);
			fwrite(&byte, 1, 1, fi);
		}
		if(bitPointer > 0)
		{
			fwrite(&work_on, 1, 1, fi);
		}
		fclose(fi);
	}
	
	void print()
	{
		for(int i = 0; i < dataSize; i++)
		{
			std::cout << i << ":\t";
			print(data->at(i));
		}
	}
	void print(int num)
	{
		for(int i = 0; i < num && i < dataSize; i++)
		{
			std::cout << i << ":\t";
			print(data->at(i));
		}
	}
	void print(LByte byte)
	{
		for(int i = 0; i < 8; i++)
		{
			std::cout << getBit(byte, i);
		}
		std::cout << std::endl;
	}
	void print(LByte byte, int index)
	{
		std::cout << getBit(byte, index) << std::endl;
	}

};

	template <class T>
	class LOrderedQueue_Node
	{
	public:
		T data;
		LOrderedQueue_Node<T>* next;
		LOrderedQueue_Node()
		{
			next = NULL;
		}
		LOrderedQueue_Node(T& data)
		{
			this->data = data;
			next = NULL;
		}
		LOrderedQueue_Node(T& data, LOrderedQueue_Node<T>* next)
		{
			this->data = data;
			this->next = next;
		}
		LOrderedQueue_Node(LOrderedQueue_Node& node)
		{
			data = node.data;
			next = node.next;
		}
		LOrderedQueue_Node(LOrderedQueue_Node* node)
		{
			data = node->data;
			next = node->next;
		}
		LOrderedQueue_Node& operator=(LOrderedQueue_Node& input)
		{
			data = input.data;
			next = input.next;
			return *this;
		}
		int operator<(LOrderedQueue_Node& input)
		{
			if(data < input.data)
			{
				return 1;
			}
			return 0;
		}
		int operator>(LOrderedQueue_Node& input)
		{
			if(data > input.data)
			{
				return 1;
			}
			return 0;
		}
		int operator<=(LOrderedQueue_Node& input)
		{
			if(data <= input.data)
			{
				return 1;
			}
			return 0;
		}
		int operator>=(LOrderedQueue_Node& input)
		{
			if(data >= input.data)
			{
				return 1;
			}
			return 0;
		}
		int operator==(LOrderedQueue_Node& input)
		{
			if(data == input.data)
			{
				return 1;
			}
			return 0;
		}
	};

	template <class T>
	class LOrderedQueue
	{
	public:
		LOrderedQueue_Node<T>* head;
		int size;
		LOrderedQueue()
		{
			head = NULL;
			size = 0;
		}
		void error(char* err_name)
		{
			std::cout << err_name << std::endl;
			system("PAUSE");
			exit(1);
		}
		~LOrderedQueue()
		{
			LOrderedQueue_Node<T>* ptr = head;
			while(ptr != NULL)
			{
				LOrderedQueue_Node<T>* tmp = ptr;
				ptr = ptr->next;
				delete tmp;
			}
			size = 0;
		}
		void add(T& input)
		{
			if(size == 0)
			{
				head = new LOrderedQueue_Node<T>(input);
				size++;
				return;
			}
			else if(input < head->data)
			{
				LOrderedQueue_Node<T>* ptr = head;
				head = new LOrderedQueue_Node<T>(input, ptr);
				size++;
				return;
			}
			else
			{
				LOrderedQueue_Node<T>* previous = head;
				LOrderedQueue_Node<T>* current = head->next;
				while(1)
				{
					if(current == NULL || current->data > input)
					{
						break;
					}

					previous = current;
					current = current->next;
				}
				LOrderedQueue_Node<T>* new_node = new LOrderedQueue_Node<T>(input, current);
				previous->next = new_node;
				size++;
			}

		}
		LOrderedQueue_Node<T>* get_node(int index)
		{
			if(index < 0 || index >= size)
			{
				error("error in LOrderedQueue.get_reference(), index out of bounds");
			}
			LOrderedQueue_Node<T>* ptr = head;
			for(int i = 0; i < index; i++)
			{
				ptr = ptr->next;
			}
			return ptr;
		}
		T& getValue_reference(int index)
		{
			if(index < 0 || index >= size)
			{
				error("error in LOrderedQueue.getValue_reference(), index out of bounds");
			}
			LOrderedQueue_Node<T>* ptr = this->get_node(index);
			return ptr->data;
		}
		T& operator[](int index)
		{
			return getValue_reference(index);
		}
		void operator+=(T& input)
		{
			add(input);
		}
		T pop_queue()
		{
			if(size <= 0)
			{
				error("error in LOrderedQueue.pop_queue() - cannot pop from an empty list");
			}
			if(size == 1)
			{
				T rv = head->data;
				delete head;
				head = NULL;
				size--;
				return rv;
			}
			else
			{
				LOrderedQueue_Node<T>* ptr = get_node(size-2);
				LOrderedQueue_Node<T>* dudNode = ptr->next;
				LOrderedQueue_Node<T>* reband = dudNode->next;
				ptr->next = reband;
				T rv = dudNode->data;
				delete dudNode;
				size--;
				return rv;
			}
		}
		T pop_stack()
		{
			if(size <= 0)
			{
				error("error in LOrderedQueue.pop_stack() - cannot pop from an empty list");
			}
			LOrderedQueue_Node<T>* ptr = head;
			head = ptr->next;
			T rv = ptr->data;
			delete ptr;
			size--;
			return rv;
		}
		T pop(int index)
		{
			if(index < 0 || index >= size)
			{
				error("error in LOrderedQueue.pop(), cannot pop that index");
			}
			if(index == 0)
			{
				return pop_stack();
			}
			else if(index == size-1)
			{
				return pop_queue();
			}
			else
			{
				LOrderedQueue_Node<T>* ptr = this->get_node(index - 1);
				LOrderedQueue_Node<T>* dudNode = ptr->next;
				ptr->next = dudNode->next;
				T rv = dudNode->data;
				delete dudNode;
				size--;
				return rv;
			}

		}
	};

	class Huffman_Compressor_Node
	{
	public:
		Huffman_Compressor_Node* left, *right;
		std::vector<LBit>* out_string; 
		float probability;
		LByte byte;
		int isLeaf;
		Huffman_Compressor_Node()
		{
			left = right = NULL;
			out_string = new std::vector<LBit>;
			probability = 0.0f;
			isLeaf = 0;
			byte = 0x00;
		}
		~Huffman_Compressor_Node()
		{
			if(out_string != NULL)
			{
				delete out_string;
				out_string = NULL;
			}
		}
		void freeMemory()
		{
			if(left != NULL)
			{
				left->freeMemory();
				delete left;
				left = NULL;
			}
			if(right != NULL)
			{
				right->freeMemory();
				delete right;
				right = NULL;
			}
			if(out_string != NULL)
			{
				delete out_string;
				out_string = NULL;
			}

		}
		void error(char* err_name)
		{
			std::cout << err_name << std::endl;
			system("PAUSE");
			exit(1);
		}
		Huffman_Compressor_Node(LByte b_in, float probability_in)
		{
			left = right = NULL;
			out_string = new std::vector<LBit>;
			if(out_string == NULL)
			{
				error("could not allocate memory in Huffman_Compressor_Node constructor");
			}
			probability = probability_in;
			isLeaf = 1;
			byte = b_in;
		}
		Huffman_Compressor_Node(Huffman_Compressor_Node& left_input, Huffman_Compressor_Node& right_input)
		{
			left = new Huffman_Compressor_Node(left_input);
			right = new Huffman_Compressor_Node(right_input);
			out_string = new std::vector<LBit>;
			if(out_string == NULL)
			{
				error("could not allocate memory in Huffman_Compressor_Node constructor");
			}
			probability = left->probability + right->probability;
			isLeaf = 0;
			byte = 0x00;
		}
		Huffman_Compressor_Node(Huffman_Compressor_Node& input)
		{
			left = input.left;
			right = input.right;
			out_string = new std::vector<LBit>;
			if(out_string == NULL)
			{
				error("could not allocate memory in Huffman_Compressor_Node constructor");
			}
			for(int i = 0; i < input.out_string->size(); i++)
			{
				out_string->push_back(input.out_string->at(i));
			}
			probability = input.probability;
			isLeaf = input.isLeaf;
			byte = input.byte;
		}
		Huffman_Compressor_Node(Huffman_Compressor_Node* input)
		{
			left = input->left;
			right = input->right;
			out_string = new std::vector<LBit>;
			if(out_string == NULL)
			{
				error("could not allocate memory in Huffman_Compressor_Node constructor");
			}
			for(int i = 0; i < input->out_string->size(); i++)
			{
				out_string->push_back(input->out_string->at(i));
			}
			probability = input->probability;
			isLeaf = input->isLeaf;
			byte = input->byte;
		}
		Huffman_Compressor_Node& operator=(Huffman_Compressor_Node& input)
		{
			delete out_string;
			left = input.left;
			right = input.right;
			out_string = new std::vector<LBit>;
			if(out_string == NULL)
			{
				error("could not allocate memory in Huffman_Compressor_Node.=()");
			}
			for(int i = 0; i < input.out_string->size(); i++)
			{
				out_string->push_back(input.out_string->at(i));
			}
			probability = input.probability;
			isLeaf = input.isLeaf;
			byte = input.byte;
			return *this;
		}
		int operator<(Huffman_Compressor_Node& input)
		{
			if(probability < input.probability)
			{
				return 1;
			}
			return 0;
		}
		int operator>(Huffman_Compressor_Node& input)
		{
			if(probability > input.probability)
			{
				return 1;
			}
			return 0;
		}
		int operator<=(Huffman_Compressor_Node& input)
		{
			if(probability <= input.probability)
			{
				return 1;
			}
			return 0;
		}
		int operator>=(Huffman_Compressor_Node& input)
		{
			if(probability >= input.probability)
			{
				return 1;
			}
			return 0;
		}
		int operator==(Huffman_Compressor_Node& input)
		{
			if(probability == input.probability)
			{
				return 1;
			}
			return 0;
		}
		void add_LBit(LBit bit)
		{
			out_string->push_back(bit);
		}
		void operator+=(LBit bit)
		{
			add_LBit(bit);
		}
		void copyIn_out_string(std::vector<LBit>* bit_list)
		{
			for(int i = 0; i < bit_list->size(); i++)
			{
				out_string->push_back(bit_list->at(i));
			}
		}
		void copyIn_out_string(std::vector<LBit>* bit_list, LBit bit)
		{
			for(int i = 0; i < bit_list->size(); i++)
			{
				out_string->push_back(bit_list->at(i));
			}
			out_string->push_back(bit);
		}
		void printString()
		{
			std::cout << "bit string: ";
			for(int i = 0; i < out_string->size(); i++)
			{
				std::cout << out_string->at(i);
			}
			std::cout << std::endl;
		}
	};

	class Huffman_Code_Map
	{
	public:
		LByte byte;
		std::vector<LBit>* code;
		Huffman_Code_Map()
		{
			byte = 0x00;
			code = new std::vector<LBit>;
		}
		~Huffman_Code_Map()
		{
			delete code;
			code = NULL;
		}
		Huffman_Code_Map(LByte by)
		{
			byte = by;
			code = new std::vector<LBit>;
		}
		Huffman_Code_Map(LByte by, std::vector<LBit>* lis)
		{
			byte = by;
			code = new std::vector<LBit>;
			for(int i = 0; i < lis->size(); i++)
			{
				code->push_back(lis->at(i));
			}
		}
		Huffman_Code_Map(Huffman_Code_Map& code_map_in)
		{
			byte = code_map_in.byte;
			code = new std::vector<LBit>;
			for(int i = 0; i < code_map_in.code->size(); i++)
			{
				code->push_back(code_map_in.code->at(i));
			}
		}
		Huffman_Code_Map(const Huffman_Code_Map& code_map_in)
		{
			byte = code_map_in.byte;
			code = new std::vector<LBit>;
			for(int i = 0; i < code_map_in.code->size(); i++)
			{
				code->push_back(code_map_in.code->at(i));
			}
		}
		Huffman_Code_Map(Huffman_Code_Map* code_map_in)
		{
			byte = code_map_in->byte;
			code = new std::vector<LBit>;
			for(int i = 0; i < code_map_in->code->size(); i++)
			{
				code->push_back(code_map_in->code->at(i));
			}
		}
		Huffman_Code_Map& operator=(Huffman_Code_Map& code_map_in)
		{
			delete code;
			byte = code_map_in.byte;
			code = new std::vector<LBit>;
			for(int i = 0; i < code_map_in.code->size(); i++)
			{
				code->push_back(code_map_in.code->at(i));
			}
			return *this;
		}
		int has_the_same_code(std::vector<LBit>* input)
		{
			if(input->size() != code->size())
			{
				return 0;
			}
			for(int i = 0; i < input->size(); i++)
			{
				if(input->at(i) != code->at(i))
				{
					return 0;
				}
			}

			return 1;
		}
		void addBit(LBit bit)
		{
			code->push_back(bit);
		}
		void operator+=(LBit bit)
		{
			addBit(bit);
		}

	};

	class Huffman_Compressor
	{
	public:
		BitWriter* output;
		BitReader* input;
		float histogram[256];
		int existance[256];
		std::vector<LBit>* conversionList[256];
		std::vector<Huffman_Code_Map>* code_map;
		Huffman_Compressor(char* input_name, char* output_name)
		{
			output = new BitWriter;
			input = new BitReader(input_name);
			code_map = new std::vector<Huffman_Code_Map>;
			if(input->dataSize <= 0)
			{
				error("Error in Huffman_Compressor Constructor, the input file is empty");
			}
			calculate_histogram();
			
			//print_histogram();
			
			Huffman_Compressor_Node* root = generate_new_tree();
			
			generate_codes(root, NULL);
			
			root->freeMemory();
			
			delete root;
			
			//print_codes();
			
			//std::cout << "CODE BOOK LENGTH: " << code_map->size() << std::endl;

			generate_conversion_list();

			save_header_file(output_name);

			save_body_file(output_name);
		}
		~Huffman_Compressor()
		{
			delete output;
			output = NULL;
			delete input;
			input = NULL;
			delete code_map;
			code_map = NULL;
		}
		void error(char* error_name)
		{
			std::cout << error_name << std::endl;
			system("PAUSE");
			exit(1);
		}
		void calculate_histogram()
		{
			input->resetPointers();
			for(int i = 0; i < 256; i++)
			{
				histogram[i] = 0.0f;
				existance[i] = 0;
			}
			float count = 0.0f;
			for(int i = 0; i < input->dataSize; i++)
			{
				LByte byte = input->readByte();
				unsigned int index = (unsigned int)byte;
				if(index >= 256)
				{
					error("Error Generating Histogram in Huffman_Compressor.calculate_histogram()");
				}
				histogram[index] += 1.0f;
				existance[index] = 1;
				count += 1.0f;
			}
			input->resetPointers();
			for(int i = 0; i < 256; i++)
			{
				histogram[i] /= count;
			}

		}
		void print_histogram()
		{
			std::cout << "Histogram: \n";
			for(int i = 0; i < 256; i++)
			{
				std::cout << "\t" << i << ". " << histogram[i] << std::endl;
			}
			std::cout << "End Histogram\n";
		}
		Huffman_Compressor_Node* generate_new_tree()
		{
			LOrderedQueue<Huffman_Compressor_Node> que;
			for(unsigned int i = 0; i < 256; i++)	//add each code to the queue
			{
				if(existance[i] != 0)
				{
					LByte b = (LByte) i;
					que += Huffman_Compressor_Node(b, histogram[i]);
				}
			}

			while(que.size > 1)
			{
				Huffman_Compressor_Node n1 = que.pop_stack();
				Huffman_Compressor_Node n2 = que.pop_stack();
				Huffman_Compressor_Node nn(n2, n1);
				que += nn;
			}
			if(que.size != 1)
			{
				error("Error in Huffman_Compressor.generate_new_tree(), tree could not be generated");
			}
			return new Huffman_Compressor_Node(que.pop_stack());
		}
		void generate_codes(Huffman_Compressor_Node* node, Huffman_Compressor_Node* prev)
		{
			if(prev == NULL)
			{
				if(node->left != NULL)
				{
					node->left->add_LBit(0);

					generate_codes(node->left, node);
				}
				if(node->right != NULL)
				{
					node->right->add_LBit(1);

					generate_codes(node->right, node);
				}
				return;
			}
			else
			{
				if(node->left != NULL)
				{
					node->left->copyIn_out_string(node->out_string, 0);

					generate_codes(node->left, node);
				}
				if(node->right != NULL)
				{
					node->right->copyIn_out_string(node->out_string, 1);

					generate_codes(node->right, node);
				}

				if(node->right == NULL && node->left == NULL && node->isLeaf == 0)
				{
					error("Error  in Huffman_Compressor.generate_codes(), non leaf node is without children");
				}
				else if((node->right != NULL || node->left != NULL) && node->isLeaf == 1)
				{
					error("Error  in Huffman_Compressor.generate_codes(), leaf node has children");
				}
				else if(node->isLeaf == 1)
				{
					
					code_map->push_back(Huffman_Code_Map(node->byte, node->out_string));
				}

				return;
			}
		}
		void print_codes()
		{
			BitWriter bw;
			for(int i = 0; i < code_map->size(); i++)
			{
				Huffman_Code_Map t = code_map->at(i);
				bw.print(t.byte);
				//std::cout << t.byte;
				std::cout << "\r\t:\t";
				for(int j = 0; j < t.code->size(); j++)
				{
					std::cout << t.code->at(j);
				}
				std::cout << std::endl;
			}
		}
		void generate_conversion_list()
		{
			for(unsigned int i = 0; i < 256; i++)
			{
				LByte b = (LByte) i;
				if(existance[i] != 0)
				{
					conversionList[i] = getCode_for_conversion_list(b);
				}
				else
				{
					conversionList[i] = NULL;
				}
			}
		}
		std::vector<LBit>* getCode_for_conversion_list(LByte b)
		{
			for(int i = 0; i < code_map->size(); i++)
			{
				if(code_map->at(i).byte == b)
				{
					return code_map->at(i).code;
				}
			}
			error("CODE MAP GENERATED INCORRECTLY in Huffman_Compressor.getCode()");
			return NULL;
		}
		void save_header_file(char* out_name)
		{
			BitWriter bw;
			int len = strlen(out_name) + 20;
			char* buffer = new char[len];
			strcpy(buffer, out_name);
			strcat(buffer, "_Huffman_Header");
			FILE* fi = fopen(buffer, "w");
			delete [] buffer;
			if(fi == NULL)
			{
				error("ERROR IN Huffman_Compressor.save_header_file(), cannot open the file");
			}

			fprintf(fi, "%u\n", (unsigned int)code_map->size());
			for(int i = 0; i < code_map->size(); i++)
			{
				Huffman_Code_Map m_e = code_map->at(i);
				LByte b1 = m_e.byte;
				for(int i = 0; i < 8; i++)
				{
					LBit bit = bw.getBit(b1, i);
					if(bit == 0)
					{
						fprintf(fi, "0");
					}
					else
					{
						fprintf(fi, "1");
					}
					
				}
				fprintf(fi, "\n");
				fprintf(fi, "%u\n", (unsigned int)m_e.code->size());
				for(int i = 0; i < m_e.code->size(); i++)
				{
					if(m_e.code->at(i) == 0)
					{
						fprintf(fi, "0");
					}
					else
					{
						fprintf(fi, "1");
					}
					
				}
				fprintf(fi, "\n");
			}


			fclose(fi);
		}
		void save_body_file(char* out_name)
		{
			BitWriter writer;
			input->resetPointers();
			for(int i = 0; i < input->dataSize; i++)
			{
				LByte byte = input->readByte();
				unsigned int index = (unsigned int) byte;
				if(index < 0 || index >= 256 || conversionList[index] == NULL)
				{
					error("ERROR IN Huffman_Compressor.save_body_file() - index out of range or code does not exist");
				}
				std::vector<LBit>* bit_list = conversionList[index];
				for(int i = 0; i < bit_list->size(); i++)
				{
					writer += bit_list->at(i);
				}
			}
			writer.save(out_name);
		}
	};

	class Huffman_Decompressor
	{
	public:
		std::vector<Huffman_Code_Map>* code_map;
		char** fast_map_i;
		LByte* fast_map_d;
		int map_len;
		Huffman_Decompressor(char* input_name, char* output_name)
		{
			code_map = new std::vector<Huffman_Code_Map>;
			read_header_file(input_name);
			//print_codes();

			generate_fast_map();

			decode_fast(input_name, output_name);

		}
		~Huffman_Decompressor()
		{
			delete [] fast_map_d;
			for(int i = 0; i < map_len; i++)
			{
				char* ptr = fast_map_i[i];
				delete [] ptr;
			}
			delete [] fast_map_i;

			delete code_map;
		}
		void error(char* error_name)
		{
			std::cout << error_name << std::endl;
			system("PAUSE");
			exit(1);
		}
		void read_header_file(char* input_name)
		{
			BitWriter bw;
			int len = strlen(input_name) + 20;
			char* buffer = new char[len];
			strcpy(buffer, input_name);
			strcat(buffer, "_Huffman_Header");
			FILE* fi = fopen(buffer, "r");
			delete [] buffer;
			if(fi == NULL)
			{
				error("ERROR IN Huffman_Decompressor.read_header_file(), cannot open the file");
			}

			unsigned int number_of_codes = 0;
			fscanf(fi, "%u\n", &number_of_codes);
			for(int i = 0; i < number_of_codes; i++)
			{
				
				LByte b1 = 0x00;
				for(int i = 0; i < 8; i++)
				{
					unsigned char bit_read = '0';
					fscanf(fi, "%c", &bit_read);
					LBit bit = (bit_read == '0')?0:1;
					bw.setBit(b1, i, bit);
						
				}
				fscanf(fi, "\n");
				Huffman_Code_Map m_e(b1);
				unsigned int code_len = 0;
				fscanf(fi, "%u\n", &code_len);
				for(int i = 0; i < code_len; i++)
				{
					unsigned char bit_read = '0';
					fscanf(fi, "%c", &bit_read);
					LBit bit = (bit_read == '0')?0:1;
					m_e += bit;
				}
				fscanf(fi, "\n");
				code_map->push_back(m_e);
			}


			fclose(fi);
		}
		void print_codes()
		{
			BitWriter bw;
			for(int i = 0; i < code_map->size(); i++)
			{
				Huffman_Code_Map t = code_map->at(i);
				bw.print(t.byte);
				//std::cout << t.byte;
				std::cout << "\r\t:\t";
				for(int j = 0; j < t.code->size(); j++)
				{
					std::cout << t.code->at(j);
				}
				std::cout << std::endl;
			}
		}
		int translate(std::vector<LBit>* from, LByte& to)
		{
			for(int i = 0; i < code_map->size(); i++)
			{
				Huffman_Code_Map c_e = code_map->at(i);
				if(c_e.has_the_same_code(from) == 1)
				{
					to = c_e.byte;
					return 1;
				}
			}
			return 0;
		}
		int translate_fast(char* from, int from_len, LByte& to)
		{
			
			for(int i = 0; i < map_len; i++)
			{
				char* ptr = fast_map_i[i];
				LByte byt = fast_map_d[i];
				if(strlen(ptr) != from_len)
				{
					continue;
				}
				if(strcmp(from, ptr) == 0)
				{
					to = byt;
					return 1;
				}
				
			}
			return 0;
		}
		int max_code_len()
		{
			int max = code_map->at(0).code->size();
			for(int i = 1; i < code_map->size(); i++)
			{
				if(code_map->at(i).code->size() > max)
				{
					max = code_map->at(i).code->size();
				}
			}
			return max;
		}
		int min_code_len()
		{
			int min = code_map->at(0).code->size();
			for(int i = 1; i < code_map->size(); i++)
			{
				if(code_map->at(i).code->size() < min)
				{
					min = code_map->at(i).code->size();
				}
			}
			return min;
		}
		void generate_fast_map()
		{
			map_len = code_map->size();
			fast_map_i = new char*[map_len];
			fast_map_d = new LByte[map_len];
			for(int i = 0; i < map_len; i++)
			{
				Huffman_Code_Map m_o = code_map->at(i);
				int stlen = m_o.code->size();
				fast_map_i[i] = new char[stlen+1];
				char* st = fast_map_i[i];
				for(int j = 0; j < stlen; j++)
				{
					unsigned char bc = (m_o.code->at(j) == 0)? '0' : '1';
					st[j] = bc;
				}
				st[stlen] = NULL;
				fast_map_d[i] = m_o.byte;


			}
		}
		void decode(char* input_name, char* output_name)
		{
			if(code_map->size() == 0)
			{
				error("ERROR IN - Huffman_Decompressor.decode() - NO CODE MAP WAS GENERATED");
			}
			int maxcl = this->max_code_len();
			int mincl = this->min_code_len();
			BitReader reader(input_name);
			BitWriter writer;

			std::vector<LBit> code_str;
			while(1)
			{
				if(reader.pointersAtEnd())
				{
					break;
				}
				LBit bit = reader.readBit();
				code_str.push_back(bit);
				if(code_str.size() > maxcl)
				{
					error("error in HuffmanDecoder.decode() - no matching code in bit stream");
				}
				if(code_str.size() < mincl)
				{
					continue;
				}
				LByte byte_out = 0x00;
				int translation_successful = translate(&code_str, byte_out);
				if(translation_successful)
				{
					code_str.clear();
					writer += byte_out;
				}
			}
			writer.save(output_name);
		}
	
		void decode_fast(char* input_name, char* output_name)
		{
			if(code_map->size() == 0)
			{
				error("ERROR IN - Huffman_Decompressor.decode() - NO CODE MAP WAS GENERATED");
			}
			int maxcl = this->max_code_len();
			int mincl = this->min_code_len();
			BitReader reader(input_name);
			BitWriter writer;
			char* buffer = new char[maxcl+20];
			int index = 0;
			
			while(1)
			{
				if(reader.pointersAtEnd())
				{
					break;
				}
				unsigned char bit_c = (reader.readBit() == 0)? '0' : '1';
				buffer[index] = bit_c;
				index++;
				buffer[index] = NULL;
				if(index > maxcl)
				{
					error("error in HuffmanDecoder.decode_fast() - no matching code in bit stream");
				}
				if(index < mincl)
				{
					continue;
				}
				LByte byte_out = 0x00;
				int translation_successful = translate_fast(buffer, index, byte_out);
				if(translation_successful)
				{
					index = 0;
					writer += byte_out;
				}
			}
			writer.save(output_name);
		}
	};

	class Huffman_Codec
	{
	public:
		Huffman_Codec(char* f_in, char* f_out, int compress0_decompress1 = 0)
		{
			if(compress0_decompress1 == 0)
			{
				Huffman_Compressor(f_in, f_out);
			}
			else
			{
				Huffman_Decompressor(f_in, f_out);
			}
		}
	};
}


#endif