//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2014
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

%MappedType MaxipixReconstruction::PositionList
{
%TypeHeaderCode
#include <list>
#include <MaxipixReconstruction.h>
using namespace Maxipix;
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   // Go through each element in the C++ instance and convert it to a
   // wrapped TYPE.
   
   int i=0;
   for (std::list<MaxipixReconstruction::Position>::iterator iter = sipCpp->begin(); iter != sipCpp->end(); iter++)
   {
       MaxipixReconstruction::Position *cpp = new MaxipixReconstruction::Position(*iter);
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       if ((pobj = sipConvertFromNewType(cpp, sipType_Maxipix_MaxipixReconstruction_Position, sipTransferObj)) == NULL)
       // one if necessary, and handle any ownership transfer.
       {
           // There was an error so garbage collect the Python list.
           Py_DECREF(l);
           return NULL;
       }

       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i++, pobj);
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL)
   {
       // Must be any iterable
       PyObject *i = PyObject_GetIter(sipPy);
       bool iterable = (i != NULL);
       Py_XDECREF(i);
       return iterable;
   }

   // Iterate over the object
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item;

   std::list<MaxipixReconstruction::Position> *l = new std::list<MaxipixReconstruction::Position>;

   while ((item = PyIter_Next(iterator)))
   {
       if (!sipCanConvertToType(item, sipType_Maxipix_MaxipixReconstruction_Position, SIP_NOT_NONE))
       {
           PyErr_Format(PyExc_TypeError, "Object in iterable cannot be converted to Position");
           *sipIsErr = 1;
           break;
       }

       int state;
       MaxipixReconstruction::Position* p = reinterpret_cast<MaxipixReconstruction::Position*>(
            sipConvertToType(item, sipType_Maxipix_MaxipixReconstruction_Position, 0, SIP_NOT_NONE, &state, sipIsErr));

       if (!*sipIsErr)
           l->push_back(*p);

       sipReleaseType(p, sipType_Maxipix_MaxipixReconstruction_Position, state);
       Py_DECREF(item);
   }

   Py_DECREF(iterator);

   if (*sipIsErr)
   {
       delete l;
       return 0;
   }

   *sipCppPtr = l;
   return sipGetState(sipTransferObj);
%End
};

namespace Maxipix
{

  class MaxipixReconstruction : LinkTask
  {
%TypeHeaderCode
#include "MaxipixReconstruction.h"
#include "lima/Exceptions.h"
using namespace lima;
using namespace Maxipix;
%End
  public:
    struct Position
    {
	RotationMode rotation;
        Point origin;

	const char* __repr__();
%MethodCode
        std::ostringstream str;
        str << *sipCpp;
        const std::string& tmpString = str.str();
        sipRes = tmpString.c_str();
%End

    };


    enum Type {RAW,ZERO,DISPATCH,MEAN};
    enum Layout {L_NONE, L_2x2, L_5x1, L_FREE, L_GENERAL};
		 
    explicit MaxipixReconstruction(Layout = L_NONE, Type = RAW);

    MaxipixReconstruction(const Maxipix::MaxipixReconstruction&);
    ~MaxipixReconstruction();
    
    void setType(Type);
    void setXnYGapSpace(int xSpace,int ySpace);
    Size getImageSize() const;
    void setChipsPosition(const MaxipixReconstruction::PositionList&);
    virtual Data process(Data &aData);

  private:
    MaxipixReconstruction(const Maxipix::MaxipixReconstruction&);
  };
}; // namespace Maxipix


