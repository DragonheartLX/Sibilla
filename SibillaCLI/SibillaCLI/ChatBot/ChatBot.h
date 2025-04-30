#pragma once

namespace scli 
{
    class ChatBot
    {
    public:
        ChatBot() = default;
        virtual ~ChatBot();

        virtual bool init();
        virtual bool handleFunc();
        virtual bool exit();

        // virtual void subscribe();
        // virtual void publish();
    private:
    
    };
}