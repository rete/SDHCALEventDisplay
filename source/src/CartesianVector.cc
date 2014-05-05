  /// \file CartesianVector.cc
/*
 *
 * CartesianVector.cc source template automatically generated by a class generator
 * Creation date : lun. avr. 28 2014
 *
 * This file is part of SDHCALEventDisplay libraries.
 *
 * SDHCALEventDisplay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * SDHCALEventDisplay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SDHCALEventDisplay.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#include "CartesianVector.h"

#include <iostream>
#include <limits>

namespace sdhcal
{

float CartesianVector::getCosOpeningAngle(const CartesianVector &rhs) const
{
	const float magnitudesSquared(this->getMagnitudeSquared() * rhs.getMagnitudeSquared());

	if (magnitudesSquared < std::numeric_limits<float>::epsilon())
		return 0.f;

	float cosTheta = this->getDotProduct(rhs) / std::sqrt(magnitudesSquared);

	if (cosTheta > 1.f)
	{
		cosTheta = 1.f;
	}
	else if (cosTheta < -1.f)
	{
		cosTheta = -1.f;
	}

	return cosTheta;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void CartesianVector::getSphericalCoordinates(float &radius, float &phi, float &theta) const
{
	const float magnitude(this->getMagnitude());

	if (std::fabs(magnitude) < std::numeric_limits<float>::epsilon())
	{
		radius = 0.f;
		phi    = 0.f;
		theta  = 0.f;

		return;
	}

	radius = magnitude;
	phi    = std::atan2(m_y, m_x);
	theta  = std::acos(m_z / radius);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void CartesianVector::getCylindricalCoordinates(float &radius, float &phi, float &z) const
{
	const float magnitude(this->getMagnitude());

	if (std::fabs(magnitude) < std::numeric_limits<float>::epsilon())
	{
		radius = 0.f;
		phi    = 0.f;
		z      = 0.f;

		return;
	}

	radius = std::sqrt(m_y * m_y + m_x * m_x);
	phi    = std::atan2(m_y, m_x);
	z      = m_z;
}

//------------------------------------------------------------------------------------------------------------------------------------------

CartesianVector CartesianVector::getUnitVector() const
{
    const float magnitude(this->getMagnitude());

    if (std::fabs(magnitude) < std::numeric_limits<float>::epsilon())
    {
     return CartesianVector(*this);
    }

    return CartesianVector(m_x / magnitude, m_y / magnitude, m_z / magnitude);
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

std::ostream &operator<<(std::ostream & stream, const CartesianVector& cartesianVector)
{
    stream  << "  x: " << cartesianVector.getX()
            << "  y: " << cartesianVector.getY()
            << "  z: " << cartesianVector.getZ()
            << " length: " << cartesianVector.getMagnitude();

    return stream;
}

}