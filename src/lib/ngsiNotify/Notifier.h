#ifndef NOTIFIER_H
#define NOTIFIER_H

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
* Author: Fermin Galan
*/

#include <map>
#include <pthread.h>

#include "ngsi9/NotifyContextAvailabilityRequest.h"
#include "ngsi10/NotifyContextRequest.h"

#include "ThreadData.h"

class Notifier {

private:
    /* This field is used to store the list of existing thread, ordered by subscrition ID
     * (the key in the map). Note that each subscription can include several threads (one
     * for each ONTIMEINTERVAL notific contition) so we are using a multimap */
    std::multimap<std::string, ThreadData> threadsMap;

public:
   
    virtual ~Notifier(void);

    virtual void sendNotifyContextRequest(NotifyContextRequest* ncr, const std::string& url, const std::string& tenant, Format format = XML);
    virtual void sendNotifyContextAvailabilityRequest(NotifyContextAvailabilityRequest* ncr, const std::string& url, const std::string& tenant, Format format = XML);
    virtual void createIntervalThread(const std::string& subId, int interval, const std::string& tenant);
    virtual void destroyOntimeIntervalThreads(const std::string& subId);

};

#endif
