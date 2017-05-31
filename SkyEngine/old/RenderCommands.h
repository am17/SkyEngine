#pragma once
#include <vector>

using namespace std;

namespace sky
{
	class IRenderCommand
	{
	public:
		virtual ~IRenderCommand() {}
		virtual void execute() = 0;
	};

	class MacroRenderCommand : public IRenderCommand
	{
	public:
		MacroRenderCommand();
		~MacroRenderCommand();
		void add(IRenderCommand *cmd) { m_cmds.push_back(cmd); }
		void execute() override
		{
			for (vector<IRenderCommand*>::const_iterator it = m_cmds.begin(); it != m_cmds.end(); ++it)
			{
				IRenderCommand *cmd = (*it);
				cmd->execute();
			}
		}
	private:
		vector<IRenderCommand*> m_cmds;
	};
}