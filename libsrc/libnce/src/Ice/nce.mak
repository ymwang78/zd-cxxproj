OUTDIR=o
OUTFILE=../../../../bin/release/libNce.so
CFG_INC=-Ibzip2 -I.. -I../../include
ALL_OBJ=$(OUTDIR)/IceStorm.o \
	$(OUTDIR)/Acceptor.o \
	$(OUTDIR)/Application.o \
	$(OUTDIR)/Buffer.o \
	$(OUTDIR)/BasicStream.o \
	$(OUTDIR)/BuiltinSequences.o \
	$(OUTDIR)/CommunicatorI.o \
	$(OUTDIR)/Communicator.o \
	$(OUTDIR)/ConnectionFactory.o \
	$(OUTDIR)/ConnectionI.o \
	$(OUTDIR)/ConnectionMonitor.o \
	$(OUTDIR)/Connection.o \
	$(OUTDIR)/Connector.o \
	$(OUTDIR)/Current.o \
	$(OUTDIR)/DefaultsAndOverrides.o \
	$(OUTDIR)/Direct.o \
	$(OUTDIR)/DynamicLibrary.o \
	$(OUTDIR)/EndpointFactoryManager.o \
	$(OUTDIR)/EndpointFactory.o \
	$(OUTDIR)/Endpoint.o \
	$(OUTDIR)/EndpointI.o \
	$(OUTDIR)/EventHandler.o \
	$(OUTDIR)/Exception.o \
	$(OUTDIR)/FacetMap.o \
	$(OUTDIR)/FactoryTableDef.o \
	$(OUTDIR)/FactoryTable.o \
	$(OUTDIR)/GC.o \
	$(OUTDIR)/Identity.o \
	$(OUTDIR)/ImplicitContextI.o \
	$(OUTDIR)/ImplicitContext.o \
	$(OUTDIR)/IncomingAsync.o \
	$(OUTDIR)/Incoming.o \
	$(OUTDIR)/Initialize.o \
	$(OUTDIR)/Instance.o \
	$(OUTDIR)/LocalException.o \
	$(OUTDIR)/LocalObject.o \
	$(OUTDIR)/LocatorInfo.o \
	$(OUTDIR)/Locator.o \
	$(OUTDIR)/LoggerI.o \
	$(OUTDIR)/Logger.o \
	$(OUTDIR)/LoggerUtil.o \
	$(OUTDIR)/Network.o \
	$(OUTDIR)/ObjectAdapterFactory.o \
	$(OUTDIR)/ObjectAdapterI.o \
	$(OUTDIR)/ObjectAdapter.o \
	$(OUTDIR)/ObjectFactoryManager.o \
	$(OUTDIR)/ObjectFactory.o \
	$(OUTDIR)/Object.o \
	$(OUTDIR)/OutgoingAsync.o \
	$(OUTDIR)/Outgoing.o \
	$(OUTDIR)/PluginManagerI.o \
	$(OUTDIR)/Plugin.o \
	$(OUTDIR)/Process.o \
	$(OUTDIR)/PropertiesI.o \
	$(OUTDIR)/Properties.o \
	$(OUTDIR)/PropertyNames.o \
	$(OUTDIR)/Protocol.o \
	$(OUTDIR)/ProtocolPluginFacade.o \
	$(OUTDIR)/ProxyFactory.o \
	$(OUTDIR)/Proxy.o \
	$(OUTDIR)/ReferenceFactory.o \
	$(OUTDIR)/Reference.o \
	$(OUTDIR)/RouterInfo.o \
	$(OUTDIR)/Router.o \
	$(OUTDIR)/ServantLocator.o \
	$(OUTDIR)/ServantManager.o \
	$(OUTDIR)/Service.o \
	$(OUTDIR)/SliceChecksumDict.o \
	$(OUTDIR)/SliceChecksums.o \
	$(OUTDIR)/Stats.o \
	$(OUTDIR)/StreamI.o \
	$(OUTDIR)/Stream.o \
	$(OUTDIR)/StringConverter.o \
	$(OUTDIR)/TcpAcceptor.o \
	$(OUTDIR)/TcpConnector.o \
	$(OUTDIR)/TcpEndpointI.o \
	$(OUTDIR)/TcpTransceiver.o \
	$(OUTDIR)/ThreadPool.o \
	$(OUTDIR)/TraceLevels.o \
	$(OUTDIR)/TraceUtil.o \
	$(OUTDIR)/Transceiver.o \
	$(OUTDIR)/UdpEndpointI.o \
	$(OUTDIR)/UdpTransceiver.o \
	$(OUTDIR)/UnknownEndpointI.o \
	$(OUTDIR)/ArgVector.o \
	$(OUTDIR)/Base64.o \
	$(OUTDIR)/Cond.o \
	$(OUTDIR)/ConvertUTF.o \
	$(OUTDIR)/CountDownLatch.o \
	$(OUTDIR)/CtrlCHandler.o \
	$(OUTDIR)/Shared.o \
	$(OUTDIR)/InputUtil.o \
	$(OUTDIR)/Options.o \
	$(OUTDIR)/OutputUtil.o \
	$(OUTDIR)/Random.o \
	$(OUTDIR)/RWRecMutex.o \
	$(OUTDIR)/RecMutex.o \
	$(OUTDIR)/StaticMutex.o \
	$(OUTDIR)/StringUtil.o \
	$(OUTDIR)/Thread.o \
	$(OUTDIR)/ThreadException.o \
	$(OUTDIR)/Time.o \
	$(OUTDIR)/UUID.o \
	$(OUTDIR)/Unicode.o \
	$(OUTDIR)/IceUtilException.o \
	$(OUTDIR)/Timer.o \
	$(OUTDIR)/SelectorThread.o \
	$(OUTDIR)/ConnectRequestHandler.o \
	$(OUTDIR)/UdpConnector.o \
	$(OUTDIR)/RequestHandler.o \
	$(OUTDIR)/ConnectionRequestHandler.o \
	$(OUTDIR)/SysLoggerI.o \
	$(OUTDIR)/blocksort.o \
	$(OUTDIR)/bzlib.o \
	$(OUTDIR)/compress.o \
	$(OUTDIR)/crctable.o \
	$(OUTDIR)/decompress.o \
	$(OUTDIR)/huffman.o \
	$(OUTDIR)/randtable.o

GCC43=$(shell expr 4.3 \<= `gcc -dumpversion | cut -f1,2 -d.`)
GCC44=$(shell expr 4.4 \<= `gcc -dumpversion | cut -f1,2 -d.`)
ifeq ($(GCC43), 1)
	C11FLAG+=-std=gnu++0x -Wno-deprecated
endif
ARCH=$(shell getconf LONG_BIT)
CFLAGMARCH32=-march=prescott
ifeq ($(GCC44), 1)
	CFLAGMARCH64=-march=core2
else
	CFLAGMARCH64=-march=nocona
endif
CFLAGMARCH=$(CFLAGMARCH$(ARCH))

COMPILE=c++ -c $(CFLAGMARCH) -DICE_API_EXPORTS -DICE_UTIL_API_EXPORTS -DICE_STORM_API_EXPORTS -D_REENTRANT -D__LINUX__ -DNODEBUG -DNDEBUG -O2 -fPIC -g $(C11FLAG) -o $(OUTDIR)/$(*F).o $(CFG_INC) $<
CCOMPILE=gcc -c $(CFLAGMARCH) -DICE_API_EXPORTS -DICE_UTIL_API_EXPORTS -DICE_STORM_API_EXPORTS -D_REENTRANT -D__LINUX__ -DNODEBUG -DNDEBUG -O2 -fPIC -g -o $(OUTDIR)/$(*F).o $(CFG_INC) $<
LINK=c++ $(CFLAGMARCH) -DICE_API_EXPORTS -DICE_UTIL_API_EXPORTS -DICE_STORM_API_EXPORTS -D_REENTRANT -D__LINUX__ -DNODEBUG -DNDEBUG -shared -fPIC -O2 -g $(C11FLAG) -o $(OUTFILE) -ldl -lpthread -lrt $(ALL_OBJ)

$(OUTDIR)/%.o : %.cpp
	$(COMPILE)

$(OUTDIR)/%.o : bzip2/%.c
	$(CCOMPILE)

all: $(OUTFILE)

$(OUTFILE): $(OUTDIR) $(ALL_OBJ)
	$(LINK)

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -fr $(OUTFILE)
	rm -fr $(ALL_OBJ)
