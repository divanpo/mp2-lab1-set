// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static size_t TELEMSize = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
    BitLen = len;
    MemLen = len / TELEMSize;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < BitLen; i++) {
        SetBit(i);
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    std::memcpy(pMem, bf.pMem, MemLen);
}

TBitField::TBitField(TBitField&& bf) noexcept {    //консруктор перемещающего копирования
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = bf.pMem;

    bf.pMem = nullptr;
    bf.BitLen = 0;
    bf.MemLen = 0;
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	1 << (n & 31); // остаток от деления на 32
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen)) {
        throw n;
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen)) {
        throw n;
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen)) {
        throw n;
    }
    int MemNum = GetMemIndex(n);
    TELEM Mask = GetMemMask(n);
    TELEM MEM = pMem[MemNum] & Mask;
    return MEM >> ((n & 31) - 1);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (*this == bf) {
        return *this;
    }
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (pMem != nullptr) {
        delete[] pMem;
    }
    pMem = new TELEM[MemLen];
    std::memcpy(pMem, bf.pMem, MemLen);
    return *this;
}

TBitField& TBitField::operator=(TBitField&& bf) noexcept { // перемещающее присваивание              (#П3)
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (pMem != nullptr) {
        delete[] pMem;
    }
    pMem = bf.pMem;

    bf.pMem = nullptr;
    bf.BitLen = 0;
    bf.MemLen = 0;
    return *this;
} 

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res = 1;
	if (BitLen != bf.BitLen)
		res = 0;
	else {
		for (int i = 0; i < BitLen; i++) {
			TELEM memMask = 0;
			if (i < MemLen) {
				memMask = 0xffffffff;
			}
			else {
				memMask = (1 << (BitLen % 32)) - 1;
			}
			if ((memMask & pMem[i]) != (memMask & bf.pMem[i])) {
				res = 0;
				break;
			}
		}
	}
	return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (len > bf.BitLen)
		len = bf.BitLen;
	TBitField tmp(len);
	for(int i = 0; i < BitLen; i++) {
		tmp.pMem[i] = pMem[i];
	}
	for(int i = 0; i < bf.BitLen; i++) {
		tmp.pMem[i] |= bf.pMem[i];
	}
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (len > bf.BitLen)
		len = bf.BitLen;
	TBitField tmp(len);
	for(int i = 0; i < MemLen; i++) {
		tmp.pMem[i] = pMem[i];
	}
	for(int i = 0; i < bf.MemLen; i++) {
		tmp.pMem[i] &= bf.pMem[i];
	}
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i << MemLen; i++) {
		pMem[i] = ~pMem[i];	
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
        if ((i != 0) || (i % 32 == 0))
            ostr << '; ';
    }
    ostr << std::endl;
    return ostr;
}
