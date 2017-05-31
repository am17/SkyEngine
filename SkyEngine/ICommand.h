#pragma once

namespace sky
{

	enum class EKEYBOARD_COMMAND
	{
		EKC_ARROWUP = 0,
		EKC_ARROWDOWN = 1,
		EKC_ARROWLEFT = 2,
		EKC_ARROWRIGHT = 3,
		EKC_W= 4,
		EKC_S = 5,
		EKC_A = 6,
		EKC_D = 7,
		EKC_F12 = 8
	};

	enum class EMOUSE_COMMAND
	{
		EMC_LEFTBNT = 0,
		EMC_RIGHTBNT = 1
	};

	class ICommand
	{
	public:
		virtual ~ICommand() {}
		virtual void execute(EKEYBOARD_COMMAND command) = 0;
		virtual void execute(EMOUSE_COMMAND command) = 0;
	};

	template<class Receiver>
	class SimpleCommand : public ICommand
	{
	public:
		typedef void(Receiver:: *Action)();
		SimpleCommand(Receiver *r, Action a)
		:m_action(a), m_receiver(r){}
		~SimpleCommand() {}
		void execute(EKEYBOARD_COMMAND command) override;
		void execute(EMOUSE_COMMAND command) override;
	private:
		Action m_action;
		Receiver *m_receiver;
	};

	template <class Receiver>
	void SimpleCommand<Receiver>::execute(EKEYBOARD_COMMAND command)
	{
		(m_receiver->*m_action)();
	}

	template <class Receiver>
	void SimpleCommand<Receiver>::execute(EMOUSE_COMMAND command)
	{
		(m_receiver->*m_action)();
	}
}
