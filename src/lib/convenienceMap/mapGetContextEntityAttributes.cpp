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
* iot_support at tid dot es
*
* Author: Ken Zangelin
*/
#include <string>

#include "logMsg/logMsg.h"

#include "convenienceMap/mapGetContextEntityAttributes.h"
#include "mongoBackend/mongoDiscoverContextAvailability.h"
#include "ngsi/StatusCode.h"
#include "ngsi9/DiscoverContextAvailabilityResponse.h"
#include "ngsi9/DiscoverContextAvailabilityRequest.h"
#include "rest/ConnectionInfo.h"
#include "rest/HttpStatusCode.h"



/* ****************************************************************************
*
* mapGetContextEntityAttributes - 
*/
HttpStatusCode mapGetContextEntityAttributes
(
  const std::string&                    id,
  DiscoverContextAvailabilityResponse*  response,
  ConnectionInfo*                       ciP
)
{
  DiscoverContextAvailabilityRequest request;
  EntityId                           entityId(id, "", "false");

  request.entityIdVector.push_back(&entityId);
  return mongoDiscoverContextAvailability(&request, response, ciP->tenant, ciP->uriParam);
}
