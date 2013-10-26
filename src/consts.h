// Copyright 2013, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef CONSTS_H
#define CONSTS_H

const char g_userAgent[] = "vkPhotoSync/1.0";
const char g_protocolVersion[] = "5.2";
const int g_defaultAppId = 1;
const int g_defaultMaximumDownloads = 5;
const int g_defaultRetryApiInterval = 500;
const int g_defaultRetryNetworkInterval = 5000;
const int g_defaultAutoSyncInterval = 60;
const bool g_defaultIsAutoSync = false;
const bool g_defaultIsSyncOnStartup = false;

#endif // CONSTS_H
