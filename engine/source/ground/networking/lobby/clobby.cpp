/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "clobby.h"
unsigned int clobby::clients = 0;
pthread_mutex_t clobby::mutex;
int clobby::listening (cxml &lobby, flobbyinstance *reference) {
    /*
        <engine>
            <lobby enabled=true>
                <address>127.0.0.1</address>
                <port>5090</port>
                <limit>1024</limit>
                <queue>8</queue>
            </lobby>
        </engine>
    */
    pthread_t link;
    pthread_attr_t attributes;
    sxmlclass *xmlsingleton = NULL;
    slobbyconfiguration *parameters = NULL;
    char *informations[ELOBBYCONFIGURATION_NULL];
    int result = EE_OK;
    informations[ELOBBYCONFIGURATION_ADDRESS] = (char *)((xmlsingleton = lobby.get("sss", "engine", "lobby", "address"))?xmlsingleton->value:EE_LOBBY_ADDRESS);
    informations[ELOBBYCONFIGURATION_PORT] = (char *)((xmlsingleton = lobby.get("sss", "engine", "lobby", "port"))?xmlsingleton->value:EE_LOBBY_PORT);
    if ((parameters = (slobbyconfiguration *) emalloc(sizeof(slobbyconfiguration)))) {
        parameters->queue = ((xmlsingleton = lobby.get("sss", "engine", "lobby", "queue"))?eatoi(xmlsingleton->value):EE_LOBBY_QUEUE);
        parameters->limit = ((xmlsingleton = lobby.get("sss", "engine", "lobby", "limit"))?eatoi(xmlsingleton->value):EE_LOBBY_LIMIT);
        parameters->reference = reference;
        if ((parameters->server = enew cserver())) {
            if ((result = parameters->server->initialize(informations[ELOBBYCONFIGURATION_ADDRESS], informations[ELOBBYCONFIGURATION_PORT], parameters->queue))) {
                clobby::clients = 0;
                pthread_mutex_init(&clobby::mutex, NULL);
                pthread_attr_init(&attributes);
                if (pthread_create(&link, &attributes, &clobby::manage, (void *)parameters) == 0)
                    pthread_detach(link);
                else
                    result = EE_ERROR;
            }
        } else
            ekill("out of memory");
    } else
        ekill("out of memory");
    return result;
}

void *clobby::manage (void *parameters) {
    pthread_t link;
    pthread_attr_t attributes;
    slobbyconfiguration *configuration = (slobbyconfiguration *)parameters;
    sclientconfiguration *subparameters = NULL;
    ssocket *client = NULL;
    bool evalutation;
    while (true) {
        if ((client = configuration->server->search())) {
            safecond((clobby::clients<configuration->limit), clobby::mutex, evalutation);
            if (evalutation) {
                pthread_attr_init(&attributes);
                if ((subparameters = (sclientconfiguration *) emalloc(sizeof(sclientconfiguration)))) {
                    subparameters->server = configuration->server;
                    subparameters->reference = configuration->reference;
                    subparameters->socket = client;
                    if (pthread_create(&link, &attributes, &clobby::connect, (void *)subparameters) == 0)
                        pthread_detach(link);
                } else
                    ekill("out of memory");
            } else
                configuration->server->unload(client);
        }
    }
    pthread_exit(NULL);
    return NULL;
}

void *clobby::connect (void *parameters) {
    sclientconfiguration *configuration = (sclientconfiguration *)parameters;
    if (configuration) {
        safemethod(clobby::clients++, clobby::mutex);
        configuration->reference(configuration->server, configuration->socket);
        configuration->server->unload(configuration->socket);
        safemethod(clobby::clients=((clobby::clients>0)?clobby::clients-1:clobby::clients), clobby::mutex);
        efree(configuration);
    }
    pthread_exit(NULL);
    return NULL;
}
