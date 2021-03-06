//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2015
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################

#include "PixelArray.h"
#include "lima/Debug.h"
#include "lima/Exceptions.h"
#include "lima/SizeUtils.h"
#include "MpxVersion.h"

#if __GNUC_MINOR_ < 6
#define nullptr 0
#endif

using namespace std;
using namespace lima;
using namespace lima::Maxipix;

PixelConfigArray::PixelConfigArray(Version version) :
		 maskArray(nullptr), testArray(nullptr), lowArray(nullptr), 
		 highArray(nullptr), m_version(version), m_bit() {
	switch (m_version) {

	case Maxipix::DUMMY:
		m_bit.mask = 0;
		m_bit.test = 1;
		m_bit.nbLow = 3;
		m_bit.low[0] = 4;
		m_bit.low[1] = 3;
		m_bit.low[2] = 2;
		m_bit.low[3] = -1;
		m_bit.nbHigh = 3;
		m_bit.high[0] = 7;
		m_bit.high[1] = 6;
		m_bit.high[2] = 5;
		m_bit.high[3] = -1;
		break;

	case Maxipix::MPX2:
		m_bit.mask = 11;
		m_bit.test = 10;
		m_bit.nbLow = 3;
		m_bit.low[0] = 9;
		m_bit.low[1] = 8;
		m_bit.low[2] = 7;
		m_bit.low[3] = -1;
		m_bit.nbHigh = 3;
		m_bit.high[0] = 12;
		m_bit.high[1] = 5;
		m_bit.high[2] = 4;
		m_bit.high[3] = -1;
		break;

	case Maxipix::MXR2:
		m_bit.mask = 0;
		m_bit.test = 9;
		m_bit.nbLow = 3;
		m_bit.low[0] = 7;
		m_bit.low[1] = 6;
		m_bit.low[2] = 8;
		m_bit.low[3] = -1;
		m_bit.nbHigh = 3;
		m_bit.high[0] = 12;
		m_bit.high[1] = 10;
		m_bit.high[2] = 11;
		m_bit.high[3] = -1;
		break;

	case Maxipix::TPX1:
		m_bit.mask = 7;
		m_bit.test = 13;
		m_bit.nbLow = 4;
		m_bit.low[0] = 8;
		m_bit.low[1] = 12;
		m_bit.low[2] = 10;
		m_bit.low[3] = 11;
		m_bit.nbHigh = 2;
		m_bit.high[0] = 9;
		m_bit.high[1] = 6;
		m_bit.high[2] = -1;
		m_bit.high[3] = -1;
		break;
	};
}

void PixelConfigArray::convert(string& buffer) {
	char maskBit[4];
	int size, base, baseMask, baseTest, baseLow[4], baseHigh[4];
	int ib, irow, wcol, wbit, idx, val;
	bool is_tpx;
	int nrow = ChipSize.getHeight();
	int ncol = ChipSize.getWidth();

	is_tpx = (m_version == Maxipix::TPX1);
	size = (ncol * nrow * 14) / 8;
	buffer.assign(size, (char) 0x00);

	for (ib = 0; ib < 4; ib++)
		maskBit[ib] = 1 << ib;

	for (irow = 0; irow < nrow; irow++) {

		base = irow * 14 * 32;
		baseMask = base + 32 * (13 - m_bit.mask);
		baseTest = base + 32 * (13 - m_bit.test);
		for (ib = 0; ib < m_bit.nbLow; ib++)
			baseLow[ib] = base + 32 * (13 - m_bit.low[ib]);
		for (ib = 0; ib < m_bit.nbHigh; ib++)
			baseHigh[ib] = base + 32 * (13 - m_bit.high[ib]);

		for (wcol = 0; wcol < 32; wcol++) {
			for (wbit = 0; wbit < 8; wbit++) {
				idx = (irow * ncol) + 255 - (wcol * 8 + wbit);
				val = 1 << (7 - wbit);

				if (maskArray[idx] & maskBit[0])
					buffer[baseMask + wcol] |= val;
				if (testArray[idx] & maskBit[0])
					buffer[baseTest + wcol] |= val;

				for (ib = 0; ib < m_bit.nbLow; ib++) {
					if (lowArray[idx] & maskBit[ib])
						buffer[baseLow[ib] + wcol] |= val;
				}
				for (ib = 0; ib < m_bit.nbHigh; ib++) {
					if (highArray[idx] & maskBit[ib])
						buffer[baseHigh[ib] + wcol] |= val;
				}
			}
		}
	}
}

PixelDataArray::PixelDataArray(){}

void PixelDataArray::convert(const string& buffer, unsigned short *data) {
	int row, dbit, wcol, dcol, wbit;
	int nrow = ChipSize.getHeight();
	int ncol = ChipSize.getWidth();

	memset(data, 0, ncol * nrow * sizeof(unsigned short));

	for (row = 0; row < nrow; row++) {
		for (dbit = 0; dbit < 14; dbit++) {
			for (wcol = 0; wcol < 32; wcol++) {
				for (wbit = 7; wbit >= 0; wbit--) {
					dcol = 255 - (wcol * 8 + wbit);
					if (buffer[row * 14 * 32 + dbit * 32 + wcol] & (1 << (7 - wbit))) {
						data[row * ncol + dcol] |= (1 << (13 - dbit));
					}
				}
			}
		}
	}
}
