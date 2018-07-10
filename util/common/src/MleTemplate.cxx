/** @defgroup MleCore Magic Lantern Runtime Engine Core Utility Library API */

/**
 * @file MleTemplate.cpp
 * @ingroup MleCore
 *
 * This file implements the classes used for generating code based on templates.
 *
 * @author Mark S. Millard
 * @date September 15, 2004
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2018 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <malloc.h>
#include <string.h>
#include <ctype.h>

// Include Magic Lantern header files.
#include "mle/MleTemplate.h"

// Define some tokens.
#define UNDERSCORE '_'
#define PERCENT_SIGN '%'
#define START_MACRO '$'
#define MACRO_BRACKET1 '{'
#define MACRO_BRACKET2 '}'
#define MACRO_ARG1 '('
#define MACRO_ARG2 ')'
#define SECTION_COMMENT "%#"
#define SECTION_FLAG "%%"

//////////////////////////////////////////////////////////////////////
// Template
//////////////////////////////////////////////////////////////////////

MleTemplate::MleTemplate()
{
    m_sectionHead = NULL;
    m_globalBindings = NULL;
}

MleTemplate::~MleTemplate()
{
    for ( MleTemplateSection *ptr = m_sectionHead ; ptr != NULL ; )
	{
		MleTemplateSection *ptr2;
		ptr2 = ptr->m_next;
		delete ptr;
		ptr = ptr2;
    }

    if ( m_globalBindings != NULL )
	{
		delete m_globalBindings;
    }
}

void MleTemplate::setGlobalBindings(MleTemplateBindings *bindings)
{
    if ( m_globalBindings != NULL )
	{
		delete m_globalBindings;
    }
    m_globalBindings = bindings;
}

int MleTemplate::read(char *name)
{
    FILE *fd;

	// Open the specified file for read-only access.
    if ((fd = fopen(name, "r")) == NULL )
	{
#if 0
	    printf ( "%s not found\n", name );
#endif
	    return -1;
    } else {
	    return read(fd);
    }
}

int MleTemplate::read(FILE *fd)
{
    char pbuf[BUFSIZ];
    MleTemplateSection *pcursect = NULL;
    
    while ( fgets(pbuf, sizeof pbuf, fd) != NULL )
	{
		int len;
		if ( (len = strlen(pbuf)) > 0 && pbuf[len-1] == '\n' )
		{
			if ( len > 1 && pbuf[len-2] == '\\' )
			{
				pbuf[len-2] = 0;
			}
		}
	
		if ( strncmp(pbuf, SECTION_COMMENT, strlen(SECTION_COMMENT)) == 0 )
		{
			continue;
		}

		if ( strncmp(pbuf, SECTION_FLAG, strlen(SECTION_FLAG)) == 0 )
		{
			// New section encountered.
			
			// Create a new current section.
			
			char *pch1, *pch2;

			// Eat white space.
			for ( pch1 = pbuf+strlen(SECTION_FLAG) ; *pch1!=0 ; pch1++ )
			{
				if ( !isspace(*pch1) )
				{
					break;
				}
			}

			if ( *pch1 != 0 )
			{
				for ( pch2 = pch1 ; *pch2 != 0 ; pch2++ )
				{
					if ( ! (isalnum(*pch2) || *pch2 == UNDERSCORE))
					{
						break;
					}
				}
				*pch2 = 0;
				if ( pch2 != pch1 )
				{
					appendSection(pcursect);
					pcursect = new MleTemplateSection(strdup(pch1));
				}
			}
	    
		} else
		{
			// Not a new section.
			if ( pcursect != NULL )
			{
				pcursect->addStrDup(pbuf);
			}
		}
    }

    if ( pcursect != NULL )
	{
		appendSection(pcursect);
    }
    return 0;
}

void MleTemplate::appendSection(MleTemplateSection *psect)
{
    if ( psect != NULL )
	{
		if ( m_sectionHead == NULL )
		{
			m_sectionHead = psect;
		} else
		{
		    m_sectionTail->m_next = psect;
		}
		m_sectionTail = psect;
    }
}

MleTemplateSection *MleTemplate::lookupSection(char *name)
{
    for (MleTemplateSection *pts = m_sectionHead ; pts != NULL ; pts = pts->m_next)
	{
		if ( strcmp(pts->m_name, name) == 0 )
		{
			return pts;
		}
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
// MleTemplateSection
//////////////////////////////////////////////////////////////////////

MleTemplateSection::MleTemplateSection(char *name)
{
    m_head = NULL;
    m_tail = NULL;
    m_next = NULL;
    m_name = strdup(name);
}

MleTemplateSection::~MleTemplateSection()
{
    for ( Line *ptr = m_head ; ptr != NULL ; )
	{
		Line *ptr2;

		if ( ptr->m_text != NULL )
		{
			free(ptr->m_text);
		}
		ptr2 = ptr;
		ptr = ptr->m_next;
		free(ptr2);
    }
}

void MleTemplateSection::addStr(char *line)
{
    Line *pseg = new Line();

    pseg->m_text = line;
    pseg->m_next = NULL;
    if ( m_head == NULL )
	{
		m_head = pseg;
    } else
	{
		m_tail->m_next = pseg;
    }
    m_tail = pseg;
}

void MleTemplateSection::addStrDup(char *line)
{
    addStr(strdup(line));
}

int MleTemplateSection::go(MleTemplateProcess *tp)
{
    int result = 0;
    for ( Line *lp = m_head ; lp != NULL ; lp = lp->m_next )
	{
		result += tp->goLine(lp->m_text);
    }
    return result;
}

int MleTemplateSection::copyOut(FILE *pfd)
{
    for ( Line *lp = m_head ; lp != NULL ; lp = lp->m_next )
	{
		fputs(lp->m_text, pfd);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////
// MleTemplateBinding
//////////////////////////////////////////////////////////////////////

MleTemplateBindings::MleTemplateBindings()
{
    m_head = NULL;
    m_tail = NULL;
}

//
// MleTemplateBindings desctuctor.
//
// NOTE: a common cause of failure here is not having th global bindings
//	table dynamically allocated, or explicitly deleting it in user
//	code.
//

MleTemplateBindings::~MleTemplateBindings()
{
    Binding *pb, *pb2;

    pb2 = NULL;
    for ( pb = m_head ; pb != NULL ; pb = pb2 )
	{
		pb2 = pb->m_next;
		delete pb;
    }
}

MleTemplateBindings::Binding::~Binding()
{
    if ( m_type == STR && m_value.m_sval != NULL )
	{
		delete m_value.m_sval;
    }
    delete m_pName;
}

void MleTemplateBindings::addBinding(Binding *pbinding)
{
    if ( m_head == NULL )
	{
		m_head = pbinding;
    } else
	{
		m_tail->m_next = pbinding;
    }
    m_tail = pbinding;
    m_tail->m_next = NULL;
}

void MleTemplateBindings::defineConstant(char *name, int _ival)
{
    removeBinding(name);
    Binding *pbinding = new Binding();
    pbinding->m_next = NULL;
    pbinding->m_type = Binding::INT;
    pbinding->m_pName = strdup(name);
    pbinding->m_value.m_ival = _ival;
    addBinding(pbinding);
}

void MleTemplateBindings::defineConstant(char *name, double _fval)
{
    removeBinding(name);
    Binding *pbinding = new Binding();
    pbinding->m_next = NULL;
    pbinding->m_type =  Binding::DOUBLE;
    pbinding->m_pName = strdup(name);
    pbinding->m_value.m_fval = _fval;
    addBinding(pbinding);
}
void MleTemplateBindings::defineConstant(char *name, char *_sval)
{
    removeBinding(name);
    Binding *pbinding = new Binding();
    pbinding->m_next = NULL;
    pbinding->m_type =  Binding::STR;
    pbinding->m_pName = strdup(name);
    pbinding->m_value.m_sval = strdup(_sval);
    addBinding(pbinding);
}

void MleTemplateBindings::defineCallback(
	char *name,
    MleTemplateBindingCallback *cb,
    void *clientdata)
{
    removeBinding(name);
    Binding *pbinding = new Binding();
    pbinding->m_next = NULL;
    pbinding->m_type =  Binding::PROC;
    pbinding->m_pName = strdup(name);
    pbinding->m_value.m_cb.m_proc = cb;
    pbinding->m_value.m_cb.m_clientdata = clientdata;
    addBinding(pbinding);
}

void MleTemplateBindings::removeBinding(char *name)
{
    Binding *pb, *prev;
    prev = NULL;
    for ( pb = m_head ; pb != NULL ; pb = pb->m_next )
	{
		if ( strcmp(name, pb->m_pName) == 0 )
		{
			break;
		}
		prev = pb;
    }
    if ( pb == NULL )
	{
		return;
    }
    if ( prev == NULL )
	{
		m_head = pb->m_next;
    } else
	{
		prev->m_next = pb->m_next;
    }
    if ( pb == m_tail )
	{
		m_tail = prev;
    }
    delete pb;
}

void MleTemplateProcess::outstr(char *str)
{
    while (*str != NULL )
	{
		outchar((int)*str);
		str++;
    }
}

void MleTemplateProcess::tabTo(int c)
{
    while ( m_column < c )
	{
		outchar(' ');
    }
}

void MleTemplateProcess::outchar(int c)
{
    fputc(c, m_pFd);
    switch ( c )
	{
      case '\n':
		m_column = 1;
		break;
      case '\t':
		m_column = ((m_column-1)/8)*8 + 8 + 1;
		break;
      default:
		m_column++;
    }
}

int MleTemplateBindings::processMacro(char *name, char *arg, MleTemplateProcess *ptp)
{
    Binding *pb;
    char cvtbuf[64];

    for ( pb = m_head ; pb != NULL ; pb = pb->m_next )
	{
		if ( strcmp(name, pb->m_pName) == 0 )
		{
			break;
		}
    }
    if ( pb == NULL )
	{
		return 0;
    }
    switch ( pb->m_type)
	{
      case Binding::INT:
		sprintf(cvtbuf, "%d", pb->m_value.m_ival);
		ptp->outstr(cvtbuf);
		break;
      case Binding::DOUBLE:
		sprintf(cvtbuf, "%g", pb->m_value.m_fval);
		ptp->outstr(cvtbuf);
		break;
      case Binding::STR:
		ptp->outstr(pb->m_value.m_sval);
		break;
      case Binding::PROC:
		(*pb->m_value.m_cb.m_proc)(ptp, arg, pb->m_value.m_cb.m_clientdata);
		break;
    }
    return 1;
}

//////////////////////////////////////////////////////////////////////
// MleTemplateProcess
//////////////////////////////////////////////////////////////////////

MleTemplateProcess::MleTemplateProcess(
    char *name,
    MleTemplate *_template,
    MleTemplateBindings *bindings,
    FILE *fd)
{
    m_pSection = name;
    m_pTemplate = _template;
    m_pBindings = bindings;
    m_pFd = fd;
    m_column = 1;
}

MleTemplateProcess::MleTemplateProcess(char *name, MleTemplateProcess *ptp)
{
    m_pSection = name;
    m_pTemplate = ptp->m_pTemplate;
    m_pBindings = ptp->m_pBindings;
    m_pFd = ptp->m_pFd;
}

MleTemplateProcess::~MleTemplateProcess()
{
	// Do nothing for now.
}

int MleTemplateProcess::go()
{
    MleTemplateSection *pcursect;

    if ( (pcursect = m_pTemplate->lookupSection(m_pSection)) == NULL )
	{
		return -1;
    }
    return pcursect->go(this);
}

//  STATE   \	$   (	)   *
//    0	    1	2   -	-   -
//    1	    -	-   -	-   -
//    2	    1	0   3	0   0
//    3	    -	-   -	0   -

int MleTemplateProcess::goLine(char *text)
{
    char *pcur;
    int state;
    char *pmacro, *pmacro0, *pmacro1, *parg, *parg0, *parg1, *pstart;
    
    state = 0;
    for ( pcur = text ; *pcur!=0 ; pcur++)
	{
	switch (state)
	{
	  case 0:
	    if (*pcur == '\\')
		{
			state = 1;
	    } else if (*pcur == START_MACRO)
		{
			pstart = pcur;
			pmacro0 = pmacro1 = NULL;
			parg0 = parg1 = NULL;
			state = 2;
	    } else
		{
			outchar((int)*pcur);
	    }
	    break;

	  case 1:
	    if (*pcur != START_MACRO)
		{
			outchar('\\');
	    }
	    outchar((int)*pcur);
	    state = 0;
	    break;

	  case 2:
	    if (*pcur == MACRO_BRACKET1)
		{
			pmacro0 = pcur+1;
			state = 3;
	    } else
		{
#define DUMP_MACRO for ( ; pstart <= pcur ; pstart++ ) { outchar((int)*pstart); }
			DUMP_MACRO
			state = 0;
	    }
	    break;

	  case 3:
	    if ( *pcur == MACRO_ARG1 )
		{
			pmacro1 = pcur-1;
			pmacro = new char[pmacro1-pmacro0+2];
			strncpy(pmacro, pmacro0, pmacro1-pmacro0+1);
			pmacro[pmacro1-pmacro0+1] = 0;
			parg0 = pcur+1;
			state = 4;
			break;
	    }
	    if (*pcur == MACRO_BRACKET2)
		{
			pmacro1 = pcur-1;
			pmacro = new char[pmacro1-pmacro0+2];
			strncpy(pmacro, pmacro0, pmacro1-pmacro0+1);
			pmacro[pmacro1-pmacro0+1] = 0;
			if (processMacro(pmacro, NULL, m_pBindings)==0)
			{
				if (processMacro(pmacro, NULL, m_pTemplate->m_globalBindings)==0 )
				{
					DUMP_MACRO;
				}
			}
			delete pmacro;
			state = 0;
			break;
	    }
	    if ( ! (isalnum(*pcur)||*pcur==UNDERSCORE) )
		{
			DUMP_MACRO;
			state = 0;
			break;
	    }
	    break;

	  case 4:
	    if ( *pcur == MACRO_ARG2 )
		{
			parg1 = pcur-1;
			parg = new char[parg1-parg0+2];
			strncpy(parg, parg0, parg1-parg0+1);
			parg[parg1-parg0+1] = 0;
			state = 5;
			break;
	    }
	    if (  ! (isalnum(*pcur)||*pcur==UNDERSCORE||*pcur==PERCENT_SIGN) )
		{
			DUMP_MACRO;
			state = 0;
			break;
	    }
	    break;

	  case 5:
	    if ( *pcur == MACRO_BRACKET2 )
		{
			if (processMacro(pmacro, parg, m_pBindings)==0)
			{
				if (processMacro(pmacro, parg, m_pTemplate->m_globalBindings)==0 )
				{
					DUMP_MACRO;
				}
			}
			if ( pmacro != NULL )
			{
				delete pmacro;
			}
			if ( parg != NULL )
			{
				delete parg;
			}
			state = 0;
			break;
		}
			DUMP_MACRO;
			state = 0;
			break;
		}
    }
    return 0;
}

int MleTemplateProcess::processMacro(char *name, char *arg, MleTemplateBindings *pb)
{
    if ( pb != NULL )
	{
		return pb->processMacro(name, arg, this);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////
// Testing Code
//////////////////////////////////////////////////////////////////////

#ifdef UNIT_TEST
#include <stdlib.h>

void testCallback(MleTemplateProcess *, char *arg, void *clientdata)
{
    printf("Test callback called\n");
    printf("arg = \"%s\"\n", arg);
    printf("client data = 0x%08x\n", clientdata);
}

void main( int argc, char **argv)
{
    FILE *fd;

    // Read the provided template file

    if ( argc != 2 )
	{
		fprintf(stderr, "Usage: %s FILE\n",argv[0]);
		exit(1);
    }
    if ( (fd = fopen(argv[1], "r")) == NULL )
	{
		perror(argv[1]);
		exit(1);
    }

    MleTemplate *pt = new MleTemplate();
    pt->read(fd);
    pt->dump(stdout);

    // Build a bindings table

    MleTemplateBindings *ptb = new MleTemplateBindings();
    ptb->defineConstant("one", 1);
    ptb->defineConstant("pi", 3.141592654);
    ptb->defineConstant("myname", "Mark Millard");
    ptb->defineCallback("proc", testCallback);
    ptb->dump(stdout);

    MleTemplateProcess *ptp = new MleTemplateProcess("FIRST", pt, ptb, stdout);
    ptp->go();
    //return 0;
}
#endif /* UNIT_TEST */

#ifdef DUMP_CODE
#define TAB fprintf(fd,"%*.*s",tab*4,tab*4," ")

void MleTemplate::dump(FILE *fd, int tab)
{
    fprintf(fd, "(TEMPLATE\n");
    tab++;
    for ( MleTemplateSection *pts = m_sectionHead ; pts!=NULL ; pts = pts->m_next)
	{
		TAB;
		fprintf(fd, "(SECTION\n");
		pts->dump(fd, tab+1);
		TAB;
		fprintf(fd, ")\n");
	}
    tab--;
    fprintf(fd, ")\n");
}

void MleTemplateSection::dump(FILE *fd, int tab)
{
    TAB;
    fprintf(fd, "(NAME \"%s\")\n", m_name);
    TAB;fprintf(fd, "(TEXT\n"); tab++;
    for ( Line *pl = m_head ; pl != NULL ; pl=pl->m_next )
	{
		TAB;fprintf(fd, "\"%s\"\n",pl->m_text);
    }
    tab--;TAB;fprintf(fd,")\n");
}

void MleTemplateBindings::dump(FILE *fd, int tab) 
{
    TAB;fprintf(fd, "(BINDINGS\n");
    tab++;
    for ( Binding *pb = m_head ; pb != NULL ; pb = pb->m_next )
	{
		TAB;fprintf(fd, "(\"%s\" ", pb->m_pName);
		switch (pb->m_type)
		{
		  case  Binding::INT:
			fprintf(fd,"'INT %d", pb->m_value.m_ival);
			break;
		  case  Binding::DOUBLE:
			fprintf(fd,"'DOUBLE %g", pb->m_value.m_fval);
			break;
		  case  Binding::STR:
			fprintf(fd,"'STRING \"%s\"", pb->m_value.m_sval);
			break;
		  case  Binding::PROC:
			fprintf(fd,"'PROC 0x%08x", (long)pb->m_value.m_cb.m_proc);
			break;
		}
		fprintf(fd, ")\n");
    }
    tab--;
    TAB;fprintf(fd, ")\n");
}
#endif /* DUMP_CODE */
