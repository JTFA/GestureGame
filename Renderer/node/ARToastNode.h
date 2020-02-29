//
// Created by kt-hyunjun on 2019-12-03.
//

#pragma once

#include "gameplay.h"
#include <string>
#include <queue>
#include <memory>
#include <model/ToastMessage.h>
#include <node/ToastFormNode.h>

namespace armakers {
    class ARToastNode : public gameplay::Node, public ToastDelegate {
    public:
        ARToastNode(const std::string &nodeId);
        virtual ~ARToastNode();

        void process(std::shared_ptr<ToastMessage> sp);
        virtual std::shared_ptr<ToastMessage> getMessage() { return queue.front(); }
        virtual void removeMessage() { queue.pop(); }
        virtual bool isEmptyMessage() { return queue.empty(); }
        virtual int32_t msgSize() { return queue.size(); }

    private:
        std::queue< std::shared_ptr<ToastMessage> > queue;
        ToastFormNode *pToastFormNode;
    };
}
