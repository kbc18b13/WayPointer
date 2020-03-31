#pragma once
#include <fstream>

class BinaryOFS{
public:
	BinaryOFS(std::ofstream& ofs) : m_ofs(ofs){}

	template<typename T>
	void writeBinary( T value ){
		m_ofs.write( reinterpret_cast<char*>( &value ), sizeof( T ) );
	}

	template<typename T>
	void writeBinary(T* value ){
		m_ofs.write( reinterpret_cast<char*>( value ), sizeof( T ) );
	}

private:
	std::ofstream& m_ofs;
};

