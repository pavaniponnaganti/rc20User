Pages {#mainpage}
============

## Introduction

Webserver is use to serve the web application GUI rendered by the DCM application (and web browsers). Webserver can also be used by the DCM to access data references that map to configuration settings on the relay using HTTP methods and WebSocket APIs. DCM can perform configuration offline (no active REL-20 is connected) and online (directly accessing an active REL-20). The following figure describe the fundamental structure of the Webserver library.
<br>
![img def1]
<br>
As described earlier, Webserver is used in DCM online and off-line device configurations. For this reason, Webserver shall be included in all applications that implement off-line configuration functions. The design assumes the application platform allows the Webserver to run. The following system model shows the relation of offline and online Webservers.
<br>
![img def2]
<br>
The Webserver package contains one library (libwebserver), the libserver library is linked to libwebsockets, libxml2, and libcjson third party libraries during compile time. The libwebserver library is also linked to libdataref and librbac (there should be an equivalent security module in the DCM or other Relay application) dynamic libraries during run-time. Consumer must make sure all dynamic libraries and their external dependencies are deployed properly to run the Webserver. Webserver is built as a static library; it is assumed it is used once in the REL-20 implementation or any other applications. The following figure shows the webserver deployment model. Webserver may link to dynamic library; it is the responsibility of the application or library to make sure all needed dynamic libraries are included. Note that some platforms (e.g. iOS) may not allow the use of dynamic library for this reason dynamic library consumed by the Webserver may be built as static instead.
<br>
![img def3]
<br>
## How to Build the Library

On libwebserver just run:
<br><br>
<code>make all</code><br>

## How to Use

To link your library to the Webserver library, add the following to your makefile:<br><br>
<code>include [path]/user/libweserver/lib/noja_webserver.mak</code><br>

To use the business rules library, include the following:<br><br>
<code>\#include "noja_webserver.h"</code><br>

Or for C++ project:<br><br>
<code>\#include "noja_webserver_cpp.h"</code><br>

Please refer to the Webserver Main APIs for the list of functions user can use.
<br>
## List of APIs

Webserver APIs are divided into to major groups Main APIs and Protocol APIs. Main APIs are used by all processes that manages the Webserver service. The Protocol APIs are APIs used by subprotocols that attached to the Relay WebSockets Protocol.

See @subpage modules
<br>


[img def1]: artefacts/01.png "Software Architecture"
[img def2]: artefacts/02.png "System Model"
[img def3]: artefacts/03.png "Deployment Model"