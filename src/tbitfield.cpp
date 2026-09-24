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
    if ((n < 0) || (n > TELEMSize)) {
        return FAKE_INT;
    }
    return (n - 1) >> 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    unsigned int number = n % 32; // or << 32 >> 32;
    TELEM Mask = 1;
    Mask = Mask << number;
    return Mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    int MemNum = GetMemIndex(n);
    if ((n < 0) || (MemNum > MemLen)) {
        return;
    }
    TELEM Mask = GetMemMask(n);
    pMem[MemNum] = pMem[MemNum] | Mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    int MemNum = GetMemIndex(n);
    if ((n < 0) || (MemNum > MemLen)) {
        return;
    }
    TELEM Mask = ~GetMemMask(n);
    pMem[MemNum] = pMem[MemNum] & Mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int MemNum = GetMemIndex(n);
    if ((n < 0) || (MemNum > MemLen)) {
        return;
    }
    TELEM Mask = GetMemMask(n);
    TELEM MEM = pMem[MemNum] & Mask;
    int answer = MEM > (n % 32);
    return answer;
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

TBitField& TBitField::operator=(TBitField&& bf) noexcept {// перемещающее присваивание              (#П3)
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
  return FAKE_INT;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return FAKE_INT;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator~(void) // отрицание
{
    return FAKE_BITFIELD;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    return ostr;
}
