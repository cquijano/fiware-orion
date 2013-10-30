/*
*
* Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
*
* This file is part of Orion Context Broker.
*
* Orion Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* fermin at tid dot es
*
* Author: Ken Zangelin
*/
#include <string>
#include <vector>

#include "logMsg/traceLevels.h"

#include "common/Format.h"
#include "common/tag.h"
#include "convenience/ContextAttributeResponseVector.h"
#include "ngsi/ErrorCode.h"
#include "convenience/AppendContextElementResponse.h"



/* ****************************************************************************
*
* AppendContextElementResponse::render - 
*/
std::string AppendContextElementResponse::render(Format format, std::string indent)
{
  std::string tag = "appendContextAttributeResponse";
  std::string out = "";

  out += startTag(indent, tag, format);

   if (errorCode.code != NO_ERROR_CODE)
     out += errorCode.render(format, indent + "  ");
   else
     out += contextResponseVector.render(format, indent + "  ");

  out += endTag(indent, tag, format);

  return out;

}



/* ****************************************************************************
*
* AppendContextElementResponse::render - 
*/
std::string AppendContextElementResponse::check(RequestType requestType, Format format, std::string indent, std::string predetectedError, int counter)
{
  return "OK";
}



/* ****************************************************************************
*
* AppendContextElementResponse::release - 
*/
void AppendContextElementResponse::release(void)
{
  LM_T(LmtRelease, ("Releasing AppendContextElementResponse"));

  contextResponseVector.release();
  errorCode.release();
}