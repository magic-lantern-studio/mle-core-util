/** @defgroup MleCore Magic Lantern Core Utility Library API */

/**
 * @file MleTemplate.h
 * @ingroup MleCore
 *
 * This file defines the classes used for generating code based on templates.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_TEMPLATE_H_
#define __MLE_TEMPLATE_H_

// Include system header files.
#include <stdio.h>

#define DUMP_CODE

#ifdef UNIT_TEST
#define DUMP_CODE
#endif

class MleTemplateBindings;
class MleTemplateSection;
class MleTemplateProcess;


/**
 * @brief This class holds information about a template file.
 *
 * The template/macro processor classes are for processing a file
 * containing "boilerplate" code and macros.  The file can be separated 
 * into sections, each beginning with a line of the form:
 * <p>
 * <b>%%SECTION_NAME</b>
 * </p><p>
 * The usage is roughly as follows:
 * </p>
 * <ol>
 * <li>
 * Create a MleTemplate object for a template file and call the
 * read() function.  This brings the file into memory and separates
 * the sections.
 * </li>
 * <li>
 * Create a MleTemplateBindings table that maps names to values, 
 * describing the macro expansions you want. There can be a "global"
 * bindings table that applies to all sections in the template, and
 * a "local" one that applies only to the section being processed.
 * </li>
 * <li>
 * Bundle the MleTemplate and MleTemplateBindings objects into a
 * MleTemplateProcess object and then call the go() function on the
 * MleTemplateProcess object.
 * </li>
 * </ol>
 * <p>
 * Macros can be bound to callbacks, and go() invokes the callback with
 * the MleTemplateProcess object as the first argument.  This allows the
 * callback to recursively process a different section by creating a new
 * MleTemplateProcess object to reference a different section.  See the 
 * constructor for MleTemplateProcess for details.
 * </p>
 */
class MleTemplate
{

    friend class MleTemplateProcess;

  public:

    /**
     * The default constructor simply initializes the class.
	 */
    MleTemplate();

    /**
     * The destructor tears down the object, freeing storage as it goes.
	 */
    virtual ~MleTemplate();

    /**
     * The read function reads in the contents of a file into internal
     * storage for future use. This read() takes the file pointer as an argument.
	 *
	 * @param fd A FILE pointer specifying the template file to read.
	 *
	 * @return An integer is returned indicating success or failure.
	 * If zero (0) is returned, then the file was successfully read.
	 * A non-zero return value will occur if the file was not successfully read.
	 */
    virtual int read(FILE *fd);

    /**
     * The read function reads in the contents of a file into internal
     * storage for future use.  This read() takes the file name as an argument.
	 *
	 * @param filename The name of the tempate file to read.
	 *
	 * @return An integer is returned indicating success or failure.
	 * If zero (0) is returned, then the file was successfully read.
	 * A non-zero return value will occur if the file was not successfully read.
     */
    virtual int read(char *filename);

	/**
	 * @brief Set the global bindings for the template.
	 *
	 * @param bindings A pointer to the global template bindings.
	 */
    void setGlobalBindings(MleTemplateBindings *bindings);

#ifdef DUMP_CODE
    void dump(FILE *fd, int tab=0);
#endif

  protected:

    /**
	 * Find the template section corresponding to the specified
	 * name.
	 *
	 * @param sectionName The name of the template section to find.
	 *
	 * @return A pointer to the found template section is returned.
	 * <b>NULL</b> will be returned if the section can not be found.
	 */
	MleTemplateSection *lookupSection(char *sectionName);

    /**
	 * @brief Append a template section to the collection of sections.
	 *
	 * @param The section to append.
	 */
	void appendSection (MleTemplateSection *section);

	/** The head of the template section list. */
    MleTemplateSection *m_sectionHead;
	/** The tail of the template section list. */
    MleTemplateSection *m_sectionTail;
	/** The global template bindings. */
    MleTemplateBindings *m_globalBindings;
};

/**
 * @brief MleTemplateSection stores a linked list of lines, zero terminated, 
 * each dynamically allocated.
 */
class MleTemplateSection
{
    friend class MleTemplate;
    friend class MleTemplateProcess;

  public:

    MleTemplateSection(char *sectionName);

    ~MleTemplateSection();

    int copyOut(FILE *fd);

    void addStr(char *str);

    void addStrDup(char *str);

    int go(MleTemplateProcess *process);

  protected:

    struct Line {
	    char *m_text;
	    Line *m_next;
    };

    char *m_name;

    Line *m_head;

    Line *m_tail;

    MleTemplateSection *m_next;

#ifdef DUMP_CODE
    void dump(FILE *fd, int tab=0);
#endif
};


/**
 * Type definition for a template bindings callback.
 */
typedef void (MleTemplateBindingCallback)(MleTemplateProcess *proccess, char *callback, void *data);

/**
 * The MleTemplateBindings stores a set of bindings from names to values.
 */
class MleTemplateBindings
{
    friend class MleTemplateSection;
    friend class MleTemplateProcess;

  public:

    MleTemplateBindings();

    ~MleTemplateBindings();

    void defineConstant(char *name, int value);

    void defineConstant(char *name, double value);

    void defineConstant(char *name, char *value);

    void defineCallback(char *name, MleTemplateBindingCallback *cb, void *clientdata = 0);

#ifdef DUMP_CODE
    void dump(FILE *fd, int tab=0);
#endif

  private:

    struct Binding
	{
	    ~Binding();

    	enum {INT, DOUBLE, STR, PROC} m_type;

	    char *m_pName;

	    union {
	        int     m_ival;
	        double  m_fval;
	        char   *m_sval;
	        struct
			{
		        MleTemplateBindingCallback *m_proc;
		        void *m_clientdata;
			} m_cb;
		} m_value;

	    Binding *m_next;
    };

    Binding *m_head;
    Binding *m_tail;

    void addBinding(Binding *binding);

    void removeBinding(char *binding);

    int processMacro(char *name, char *, MleTemplateProcess *process);
};

/**
 * @brief The MleTemplateProcess stores state information for the processing of a
 * template.  It exists to handle recursive calls to the process function
 * for MleTemplate.
 */
class MleTemplateProcess
{
    friend class MleTemplateSection;
    friend class MleTemplateBindings;

  public:

    MleTemplateProcess(char *name, MleTemplateProcess *ptp);

    MleTemplateProcess(char *name, MleTemplate *t, MleTemplateBindings *bindings, FILE *fd);

    ~MleTemplateProcess();

    int go();

    MleTemplate *getTemplate()
	{ return m_pTemplate; }

    FILE *getFileDescriptor()
	{ return m_pFd; }

    void outchar(int c);

    void outstr(char *str);

    int getColumn()
	{ return m_column; }

    void tabTo(int num);

  private:

    FILE *m_pFd;

    MleTemplate *m_pTemplate;

    MleTemplateBindings *m_pBindings;

    char *m_pSection;

    int m_column;

    int goLine(char *lineno);

    int processMacro(char *name, char *, MleTemplateBindings *bindings);
};

#endif /* __MLE_TEMPLATE_H_ */
