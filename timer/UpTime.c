#include "UpTime.h"

UpTime* UpTimeCreate(Bool singleThreaded)
{
	ENTRY;
	UpTime* m_uptime = (UpTime*) malloc (sizeof(UpTime));
	if (!m_uptime)
	{
		#ifdef DEBUG
		printf("uptime malloc failed\n");
		#endif
		return NULL;
	}

	if (!singleThreaded)
	{
		m_uptime->m_uptimeFpReadMutex = (Mutex*)malloc(sizeof(Mutex));
		InitialiseMutex(m_uptime->m_uptimeFpReadMutex);
	}
	else
	{
		m_uptime->m_uptimeFpReadMutex = NULL;
	}
	
	m_uptime->m_uptimeFp = fopen(m_fileName,"r");
	if (!m_uptime->m_uptimeFp)
	{
		#ifdef DEBUG
		printf("failed to open handle to /proc/uptime\n");
		#endif
	}
	else
	{
		SetCloExec(fileno(m_uptime->m_uptimeFp));
	}

	m_uptime->singleThreaded = singleThreaded;
	return 	m_uptime;
}

void UpTimeDestroy(UpTime** m_uptime)
{
	ENTRY;
	if ((*m_uptime)->singleThreaded != 1)
	{
		DestroyMutex((*m_uptime)->m_uptimeFpReadMutex);
		free((*m_uptime)->m_uptimeFpReadMutex);
		(*m_uptime)->m_uptimeFpReadMutex = NULL;
	}

	if ((*m_uptime)->m_uptimeFp)
	{
		fclose((*m_uptime)->m_uptimeFp);
		(*m_uptime)->m_uptimeFp = NULL;
	}
	Exit;
}

uptime_t GetMillisecs(UpTime* m_uptime)
{
	ENTRY;
	unsigned int seconds = 0;
	unsigned int hundredths = 0;

	if (m_uptime->singleThreaded != 1)
	{
		LockMutex(m_uptime->m_uptimeFpReadMutex);
		ReadUpTimeFile(m_uptime,&seconds,&hundredths);
		UnlockMutex(m_uptime->m_uptimeFpReadMutex);
	}
	else
	{
		ReadUpTimeFile(m_uptime,&seconds,&hundredths);
	}
	uptime_t uptime;
	uptime = seconds;
	uptime *= 1000;
	uptime += (uptime_t)(hundredths) * 10; // why ,uptime_t is a func????

	return uptime;

}

void ReadUpTimeFile(UpTime* m_uptime, unsigned int* seconds, unsigned int* hundredths)
{
	ENTRY;
	do
	{
		m_uptime->m_uptimeFp = freopen(m_fileName,"r",m_uptime->m_uptimeFp);
		SetCloExec(fileno(m_uptime->m_uptimeFp));
		rewind(m_uptime->m_uptimeFp);

	}while(2 != fscanf(m_uptime->m_uptimeFp,"%u.%u",seconds,hundredths));
}

void SetCloExec(int fd)
{
	ENTRY;
	int flags = fcntl(fd,F_GETFD);
	if (flags != -1)
	{
		fcntl(fd,F_SETFD,flags | FD_CLOEXEC);
	}
}

// int main()
// {
// 	UpTime* m_uptime = UpTimeCrete(0);
// 	uptime_t nowtime = GetMillisecs(m_uptime);
// 	printf("%llu\n",nowtime);
// }