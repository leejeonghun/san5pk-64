1 pascal CoBuildVersion() CoBuildVersion16
2 pascal CoInitialize(long) CoInitialize16
3 pascal CoUninitialize() CoUninitialize16
4 pascal CoGetMalloc(long ptr) CoGetMalloc16
5 pascal CoRegisterClassObject(ptr segptr long long ptr) CoRegisterClassObject16
6 pascal CoRevokeClassObject(long) CoRevokeClassObject16
7 pascal CoGetClassObject(segptr long ptr segptr segptr) CoGetClassObject16
8 stub COMARSHALINTERFACE
9 stub COUNMARSHALINTERFACE
10 stub COLOADLIBRARY
11 stub COFREELIBRARY
12 stub COFREEALLLIBRARIES
13 pascal CoCreateInstance(segptr segptr long segptr segptr) CoCreateInstance16
14 pascal StringFromIID(ptr ptr) StringFromCLSID16
15 pascal CoDisconnectObject(segptr long) CoDisconnectObject16
16 stub CORELEASEMARSHALDATA
17 pascal -ret16 CoFreeUnusedLibraries() CoFreeUnusedLibraries16
18 pascal -ret16 IsEqualGUID(ptr ptr) IsEqualGUID16
19 pascal StringFromCLSID(ptr ptr) StringFromCLSID16
20 pascal CLSIDFromString(str ptr) CLSIDFromString16
21 stub ISVALIDPTRIN
22 stub ISVALIDPTROUT
23 pascal IsValidInterface(segptr) IsValidInterface16
24 stub ISVALIDIID
25 stub RESULTFROMSCODE
26 stub GETSCODE
27 pascal CoRegisterMessageFilter(segptr ptr) CoRegisterMessageFilter16
28 stub COISHANDLERCONNECTED
29 stub SHRADDREF
30 pascal -ret16 CoFileTimeToDosDateTime(ptr ptr ptr) CoFileTimeToDosDateTime16
31 pascal -ret16 CoDosDateTimeToFileTime(word word ptr) CoDosDateTimeToFileTime16
32 stub COMARSHALHRESULT
33 stub COUNMARSHALHRESULT
34 pascal CoGetCurrentProcess() CoGetCurrentProcess16
35 stub SHRCREATE
36 pascal CoIsOle1Class(ptr) CoIsOle1Class16
37 variable _GUID_NULL(0x00000000 0x00000000 0x00000000 0x00000000)
38 variable _IID_IUnknown(0x00000000 0x00000000 0x000000c0 0x46000000)
39 variable _IID_IClassFactory(0x00000001 0x00000000 0x000000c0 0x46000000)
40 variable _IID_IMalloc(0x00000002 0x00000000 0x000000c0 0x46000000)
41 variable _IID_IMarshal(0x00000003 0x00000000 0x000000c0 0x46000000)
42 variable _IID_IRpcChannel(0x00000004 0x00000000 0x000000c0 0x46000000)
43 variable _IID_IRpcStub(0x00000005 0x00000000 0x000000c0 0x46000000)
44 variable _IID_IStubManager(0x00000006 0x00000000 0x000000c0 0x46000000)
45 variable _IID_IRpcProxy(0x00000007 0x00000000 0x000000c0 0x46000000)
46 variable _IID_IProxyManager(0x00000008 0x00000000 0x000000c0 0x46000000)
47 variable _IID_IPSFactory(0x00000009 0x00000000 0x000000c0 0x46000000)
48 variable _IID_ILockBytes(0x0000000a 0x00000000 0x000000c0 0x46000000)
49 variable _IID_IStorage(0x0000000b 0x00000000 0x000000c0 0x46000000)
50 variable _IID_IStream(0x0000000c 0x00000000 0x000000c0 0x46000000)
51 variable _IID_IEnumSTATSTG(0x0000000d 0x00000000 0x000000c0 0x46000000)
52 variable _IID_IBindCtx(0x0000000e 0x00000000 0x000000c0 0x46000000)
53 variable _IID_IMoniker(0x0000000f 0x00000000 0x000000c0 0x46000000)
54 variable _IID_IRunningObjectTable(0x00000010 0x00000000 0x000000c0 0x46000000)
55 variable _IID_IInternalMoniker(0x00000011 0x00000000 0x000000c0 0x46000000)
56 variable _IID_IRootStorage(0x00000012 0x00000000 0x000000c0 0x46000000)
57 variable _IID_IDfReserved1(0x00000013 0x00000000 0x000000c0 0x46000000)
58 variable _IID_IDfReserved2(0x00000014 0x00000000 0x000000c0 0x46000000)
59 variable _IID_IDfReserved3(0x00000015 0x00000000 0x000000c0 0x46000000)
60 variable _IID_IMessageFilter(0x00000016 0x00000000 0x000000c0 0x46000000)
61 pascal CLSIDFromProgID(str ptr) CLSIDFromProgID16
62 pascal ProgIDFromCLSID(ptr ptr) ProgIDFromCLSID16
63 pascal CoLockObjectExternal(segptr word word) CoLockObjectExternal16
64 variable _CLSID_StdMarshal(0x00000017 0x00000000 0x000000c0 0x46000000)
65 pascal CoGetTreatAsClass(ptr ptr) CoGetTreatAsClass16
66 pascal CoTreatAsClass(ptr ptr) CoTreatAsClass16
67 stub COGETSTANDARDMARSHAL
68 stub PROPAGATERESULT
69 pascal IIDFromString(str ptr) IIDFromString16
70 variable _IID_IStdMarshalInfo(0x00000018 0x00000000 0x000000c0 0x46000000)
71 pascal CoCreateStandardMalloc(long ptr) CoCreateStandardMalloc16
72 variable _IID_IExternalConnection(0x00000019 0x00000000 0x000000c0 0x46000000)
73 pascal CoCreateGuid(ptr) CoCreateGuid16
75 pascal FnAssert(str str str word) FnAssert16
76 pascal -ret16 StringFromGUID2(ptr ptr word) StringFromGUID216
77 stub COGETCLASSEXT
78 stub OLE1CLASSFROMCLSID2
79 stub CLSIDFROMOLE1CLASS
80 stub COOPENCLASSKEY
81 stub GUIDFROMSTRING
82 pascal CoFileTimeNow(ptr) CoFileTimeNow16
83 stub REMALLOCOID
84 stub REMFREEOID
85 stub REMCREATEREMOTEHANDLER
86 stub REMCONNECTTOOBJECT
87 stub REMGETINFOFORCID
88 stub LRPCCALL
89 stub LRPCDISPATCH
90 stub LRPCREGISTERMONITOR
91 stub LRPCREVOKEMONITOR
92 stub LRPCGETTHREADWINDOW
93 stub TIMERCALLBACKPROC
94 pascal LookupETask(ptr ptr) LookupETask16
95 pascal -ret16 SetETask(word ptr) SetETask16
96 stub LRPCFREEMONITORDATA
97 stub REMLOOKUPSHUNK
98 stub SHRGETSIZE
99 stub CALLTHKMGRUNINITIALIZE
100 stub ??0CARRAYFVALUE@@REC@KI@Z
101 stub ??1CARRAYFVALUE@@REC@XZ
102 stub ?ASSERTVALID@CARRAYFVALUE@@RFCXXZ
103 stub ?FREEEXTRA@CARRAYFVALUE@@RECXXZ
104 stub ?_GETAT@CARRAYFVALUE@@RFCPEXH@Z
105 stub ?GETSIZE@CARRAYFVALUE@@RFCHXZ
106 stub ?REMOVEALL@CARRAYFVALUE@@RECXXZ
107 stub SHRDESTROY
108 stub ?INDEXOF@CARRAYFVALUE@@RECHPEXII@Z
109 stub ?INSERTAT@CARRAYFVALUE@@RECHHPEXH@Z
110 stub COSETSTATE
111 stub ?REMOVEAT@CARRAYFVALUE@@RECXHH@Z
112 stub ?SETAT@CARRAYFVALUE@@RECXHPEX@Z
113 stub ?SETATGROW@CARRAYFVALUE@@RECHHPEX@Z
114 stub ?SETSIZE@CARRAYFVALUE@@RECHHH@Z
115 pascal CoGetState(ptr) CoGetState16
116 pascal DllEntryPoint(long word word word long word) COMPOBJ_DllEntryPoint
117 stub ?RELEASE@CSTDMALLOC@@VEAKXZ
118 stub ?ALLOC@CSTDMALLOC@@VEAPEXK@Z
119 stub SHRRELEASE
120 stub ?GETASSOCAT@CMAPKEYTOVALUE@@BFCPEUCASSOC@1@PEXIAEI@Z
121 stub ?SETASSOCKEY@CMAPKEYTOVALUE@@BFCHPEUCASSOC@1@PEXI@Z
122 stub ??1CMAPKEYTOVALUE@@REC@XZ
123 stub ?GETASSOCKEYPTR@CMAPKEYTOVALUE@@BFCXPEUCASSOC@1@PEPEXPEI@Z
124 stub ?NEWASSOC@CMAPKEYTOVALUE@@BECPEUCASSOC@1@IPEXI0@Z
125 stub ?SIZEASSOC@CMAPKEYTOVALUE@@BFCIXZ
126 stub ?FREEASSOC@CMAPKEYTOVALUE@@BECXPEUCASSOC@1@@Z
127 stub ?GETSTARTPOSITION@CMAPKEYTOVALUE@@RFCPEXXZ
128 stub ?GETNEXTASSOC@CMAPKEYTOVALUE@@RFCXPEPEXPEXPEI1@Z
129 stub ?COMPAREASSOCKEY@CMAPKEYTOVALUE@@BFCHPEUCASSOC@1@PEXI@Z
130 stub ?REMOVEHKEY@CMAPKEYTOVALUE@@RECHK@Z
131 stub ?GETHKEY@CMAPKEYTOVALUE@@RFCKPEXI@Z
132 stub ?GETCOUNT@CMAPKEYTOVALUE@@RFCHXZ
133 stub ?LOOKUP@CMAPKEYTOVALUE@@RFCHPEXI0@Z
134 stub ?GETASSOCVALUE@CMAPKEYTOVALUE@@BFCXPEUCASSOC@1@PEX@Z
135 stub ?REMOVEKEY@CMAPKEYTOVALUE@@RECHPEXI@Z
136 stub ?REMOVEALL@CMAPKEYTOVALUE@@RECXXZ
137 stub SHRALLOC
138 stub ?FREEASSOCKEY@CMAPKEYTOVALUE@@BFCXPEUCASSOC@1@@Z
139 stub ?SETAT@CMAPKEYTOVALUE@@RECHPEXI0@Z
140 stub ?LOOKUPHKEY@CMAPKEYTOVALUE@@RFCHKPEX@Z
141 stub ?ASSERTVALID@CMAPKEYTOVALUE@@RFCXXZ
142 stub ?SETASSOCVALUE@CMAPKEYTOVALUE@@BFCXPEUCASSOC@1@PEX@Z
143 stub ?SETATHKEY@CMAPKEYTOVALUE@@RECHKPEX@Z
144 stub ??0CMAPKEYTOVALUE@@REC@KIIHP7CIPEXI@ZI@Z
145 stub ?INITHASHTABLE@CMAPKEYTOVALUE@@BECHXZ
146 stub ?GETASSOCVALUEPTR@CMAPKEYTOVALUE@@BFCXPEUCASSOC@1@PEPEX@Z
147 stub ?LOOKUPADD@CMAPKEYTOVALUE@@RFCHPEXI0@Z
148 stub MKVDEFAULTHASHKEY
149 stub DELETE16
150 stub COMEMCTXOF
151 pascal CoMemAlloc(long long long)
152 stub COMEMFREE
153 stub SHRREALLOC
154 stub ___EXPORTEDSTUB
155 stub LRPCREGISTERWIN32SMONITOR
156 stub MYREMGETINFOFORCID
157 stub SHRFREE
158 stub OPNEW16
159 stub ADDCOINFO
160 stub CORUNMODALLOOP
161 stub COHANDLEINCOMINGCALL
162 stub COSETACKSTATE
163 stub SHRDIDALLOC
164 stub ?GETAT@CARRAYFVALUE@@RFCPEXH@Z
165 stub ?GETUPPERBOUND@CARRAYFVALUE@@RFCHXZ
166 stub OPDELETE16
167 stub ?GETSIZEVALUE@CARRAYFVALUE@@RFCHXZ
168 stub ?PROXY1632ADDREF@@ZAKPEVCPROXY1632@@@Z
# FIXME: 169 is a duplicate of 97
169 stub REMLOOKUPSHUNK_dup
170 stub ?ISEMPTY@CMAPKEYTOVALUE@@RFCHXZ
171 stub ?FREE@CSTDMALLOC@@VEAXPEX@Z
172 stub CALLTHKMGRINITIALIZE
173 stub ?REALLOC@CSTDMALLOC@@VEAPEXPEXK@Z
174 stub ?SM16RHQI@@ZAPEXPEVCSM16RELEASEHANDLER@@AFUGUID@@PEPEX@Z
175 stub ?PROXY1632METHOD10@@ZAKPEVCPROXY1632@@@Z
# FIXME: 176 is a duplicate of 154
176 stub ___EXPORTEDSTUB_dup
177 stub ?PROXY1632METHOD20@@ZAKPEVCPROXY1632@@@Z
178 stub ?PROXY1632METHOD11@@ZAKPEVCPROXY1632@@@Z
179 stub ?PROXY1632METHOD30@@ZAKPEVCPROXY1632@@@Z
180 stub ?PROXY1632METHOD21@@ZAKPEVCPROXY1632@@@Z
181 stub ?PROXY1632METHOD12@@ZAKPEVCPROXY1632@@@Z
182 stub ?PROXY1632METHOD31@@ZAKPEVCPROXY1632@@@Z
183 stub ?PROXY1632METHOD22@@ZAKPEVCPROXY1632@@@Z
184 stub ?PROXY1632METHOD13@@ZAKPEVCPROXY1632@@@Z
185 stub ?GETSIZE@CSTDMALLOC@@VEAKPEX@Z
186 stub ?PROXY1632METHOD23@@ZAKPEVCPROXY1632@@@Z
187 stub ?PROXY1632METHOD14@@ZAKPEVCPROXY1632@@@Z
188 stub ?PROXY1632METHOD24@@ZAKPEVCPROXY1632@@@Z
189 stub ?PROXY1632METHOD15@@ZAKPEVCPROXY1632@@@Z
190 stub ?PROXY1632METHOD25@@ZAKPEVCPROXY1632@@@Z
191 stub ?PROXY1632METHOD16@@ZAKPEVCPROXY1632@@@Z
192 stub ?PROXY1632METHOD26@@ZAKPEVCPROXY1632@@@Z
193 stub ?PROXY1632METHOD17@@ZAKPEVCPROXY1632@@@Z
194 stub ?PROXY1632METHOD27@@ZAKPEVCPROXY1632@@@Z
195 stub ?PROXY1632METHOD18@@ZAKPEVCPROXY1632@@@Z
196 stub ?PROXY1632METHOD28@@ZAKPEVCPROXY1632@@@Z
197 stub ?ADDREF@CSTDMALLOC@@VEAKXZ
198 stub ?PROXY1632METHOD19@@ZAKPEVCPROXY1632@@@Z
199 stub ?PROXY1632METHOD29@@ZAKPEVCPROXY1632@@@Z
200 stub CALL32INITIALIZE
201 pascal CALLOBJECTINWOW(ptr ptr) CallObjectInWOW
203 stub CALLOBJECTINWOWCHECKINIT
204 stub CALLOBJECTINWOWCHECKTHKMGR
205 stub CONVERTHR1632
206 stub CONVERTHR3216
207 stub ADDAPPCOMPATFLAG

# WINE internal relays (for Win16 interfaces)
500 cdecl IMalloc16_QueryInterface(ptr ptr ptr) IMalloc16_fnQueryInterface
501 cdecl IMalloc16_AddRef(ptr) IMalloc16_fnAddRef
502 cdecl IMalloc16_Release(ptr) IMalloc16_fnRelease
503 cdecl IMalloc16_Alloc(ptr long) IMalloc16_fnAlloc
504 cdecl IMalloc16_Realloc(ptr segptr long) IMalloc16_fnRealloc
505 cdecl IMalloc16_Free(ptr segptr) IMalloc16_fnFree
506 cdecl IMalloc16_GetSize(ptr segptr) IMalloc16_fnGetSize
507 cdecl IMalloc16_DidAlloc(ptr segptr) IMalloc16_fnDidAlloc
508 cdecl IMalloc16_HeapMinimize(ptr) IMalloc16_fnHeapMinimize

@ stdcall -arch=win32 get_task_imalloc16(ptr)
@ stdcall -arch=win32 CoInitialize16(long)
@ stdcall -arch=win32 CoUninitialize16()
@ stdcall -arch=win32 StringFromGUID216(ptr ptr long)
@ stdcall -arch=win32 CoCreateInstance16(long long long long long)