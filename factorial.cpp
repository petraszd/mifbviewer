/******************************************************************************
 *   Copyright (C) 2009 by Petras Zdanavičius                                 *
 *                                                                            *
 *   This program is free software: you can redistribute it and/or modify     *
 *   the Free Software Foundation, either version 3 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public License *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 ******************************************************************************/
#include "factorial.h"

Factorial::Factorial(unsigned int n): m_numbers(0)
{
    unsigned int number = 1;
    m_numbers = new unsigned int[n];
    for(unsigned int i = 1; i < n + 1; ++i) {
        m_numbers[i-1] = number;
        number *= i;
    }
}

Factorial::~Factorial()
{
    delete[] m_numbers;
}
