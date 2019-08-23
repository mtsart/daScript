#include "daScript/misc/platform.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

#include "daScript/ast/ast.h"
#include "daScript/simulate/simulate_fusion.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/simulate_visit_op.h"

namespace das {

    SimNode * SimNode_Op1Fusion::visit(SimVisitor & vis) {
        V_BEGIN();
        string name = op;
        name += getSimSourceName(subexpr.type);
        if (baseType != Type::none) {
            vis.op(name.c_str(), getTypeBaseSize(baseType), das_to_string(baseType));
        } else {
            vis.op(name.c_str());
        }
        subexpr.visit(vis);
        V_END();
    }

    SimNode * SimNode_Op2Fusion::visit(SimVisitor & vis) {
        V_BEGIN();
        string name = op;
        name += getSimSourceName(l.type);
        name += getSimSourceName(r.type);
        if (baseType != Type::none) {
            vis.op(name.c_str(), getTypeBaseSize(baseType), das_to_string(baseType));
        } else {
            vis.op(name.c_str());
        }
        l.visit(vis);
        r.visit(vis);
        V_END();
    }

    const char * getSimSourceName(SimSourceType st) {
        switch ( st ) {
        case SimSourceType::sSimNode:                          return "Any";
        case SimSourceType::sConstValue:                       return "Const";
        case SimSourceType::sCMResOff:                         return "Cmro";
        case SimSourceType::sBlockCMResOff:                    return "Bcmro";
        case SimSourceType::sLocal:                            return "Loc";
        case SimSourceType::sLocalRefOff:                      return "Locro";
        case SimSourceType::sArgument:                         return "Arg";
        case SimSourceType::sArgumentRefOff:                   return "Argo";
        case SimSourceType::sBlockArgument:                    return "Barg";
        case SimSourceType::sBlockArgumentRefOff:              return "Bargo";
        case SimSourceType::sThisBlockArgument:                return "Tbarg";
        case SimSourceType::sThisBlockArgumentRefOff:          return "Tbargo";
        case SimSourceType::sGlobal:                           return "Glob";
        };
        return "???";
    }

    string fuseName ( const string & name, const string & typeName ) {
        return typeName.empty() ? name : (name + "<" + typeName + ">");
    }

    unique_ptr<FusionEngine> g_fusionEngine;

    void resetFusionEngine() {
        g_fusionEngine.reset();
    }

    void createFusionEngine() {
        if ( !g_fusionEngine ) {
            g_fusionEngine = make_unique<FusionEngine>();
            // misc (note, misc before everything)
            createFusionEngine_misc_copy_reference();
            // op1
            createFusionEngine_op1();
            // scalar
            createFusionEngine_op2();
            createFusionEngine_op2_set();
            createFusionEngine_op2_bool();
            createFusionEngine_op2_bin();
            // vector
            createFusionEngine_op2_vec();
            createFusionEngine_op2_set_vec();
            createFusionEngine_op2_bool_vec();
            createFusionEngine_op2_bin_vec();
            createFusionEngine_op2_scalar_vec();
            // at
            createFusionEngine_at();
        }
    }

    struct SimNodeCollector : SimVisitor {
        virtual void preVisit ( SimNode * node ) override {
            SimVisitor::preVisit(node);
            thisNode = node;
        }
        virtual void op ( const char * name, size_t typeSize, const string & typeName ) override {
            SimNodeInfo ni;
            ni.name = name;
            ni.typeName = typeName;
            ni.typeSize = typeSize;
            info[thisNode] = ni;

        }
        map<SimNode *,SimNodeInfo>  info;
        SimNode * thisNode = nullptr;
    };

    struct SimFusion : SimVisitor {
        SimFusion ( Context * ctx, TextWriter & wr,  map<SimNode *,SimNodeInfo> && ni )
            : context(ctx), ss(wr), info(ni) {
                createFusionEngine();
        }
        void fuse() {
            fused = true;
        }
        virtual SimNode * visit ( SimNode * node ) override {
            auto & ni = info[node];
            auto & nv = (*g_fusionEngine)[fuseName(ni.name, ni.typeName)];
            for ( const auto & fe : nv ) {
                auto newNode = fe->fuse(info, node, context);
                if ( newNode != node ) {
                    fuse();
                    return newNode;
                }
            }
            return SimVisitor::visit(node);
        }
        Context * context = nullptr;
        TextWriter & ss;
        bool fused = false;
        map<SimNode *,SimNodeInfo> & info;
    };

    void Program::fusion ( Context & context, TextWriter & logs ) {
        // log all functions
        if ( options.getOption("fusion",true) ) {
            bool anyFusion = true;
            while ( anyFusion) {
                anyFusion = false;
                for ( int i=0; i!=context.totalFunctions; ++i ) {
                    SimFunction * fn = context.getFunction(i);
                    SimNodeCollector collector;
                    fn->code->visit(collector);
                    SimFusion fuse(&context, logs, move(collector.info));
                    fn->code = fn->code->visit(fuse);
                    anyFusion |= fuse.fused;
                }
            }
        }
    }
}

