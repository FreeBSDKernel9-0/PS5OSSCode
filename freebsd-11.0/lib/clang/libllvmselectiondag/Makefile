# $FreeBSD: releng/11.0/lib/clang/libllvmselectiondag/Makefile 296417 2016-03-05 20:24:31Z dim $

.include <bsd.own.mk>

LIB=	llvmselectiondag

SRCDIR=	lib/CodeGen/SelectionDAG
SRCS=	DAGCombiner.cpp \
	FastISel.cpp \
	FunctionLoweringInfo.cpp \
	InstrEmitter.cpp \
	LegalizeDAG.cpp \
	LegalizeFloatTypes.cpp \
	LegalizeIntegerTypes.cpp \
	LegalizeTypes.cpp \
	LegalizeTypesGeneric.cpp \
	LegalizeVectorOps.cpp \
	LegalizeVectorTypes.cpp \
	ResourcePriorityQueue.cpp \
	ScheduleDAGFast.cpp \
	ScheduleDAGRRList.cpp \
	ScheduleDAGSDNodes.cpp \
	ScheduleDAGVLIW.cpp \
	SelectionDAG.cpp \
	SelectionDAGBuilder.cpp \
	SelectionDAGDumper.cpp \
	SelectionDAGISel.cpp \
	SelectionDAGPrinter.cpp \
	StatepointLowering.cpp \
	TargetLowering.cpp \
	TargetSelectionDAGInfo.cpp

TGHDRS=	Attributes \
	Intrinsics

.include "../clang.lib.mk"
