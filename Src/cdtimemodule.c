/* -*-Mode: C;-*-
 * Module:      cdtime Python module
 *
 * Copyright:	1997, Regents of the University of California
 *		This software may not be distributed to others without
 *		permission of the author.
 *
 * Author:      Bob Drach, Lawrence Livermore National Laboratory
 *              drach@llnl.gov
 *
 * Version:     $Id$
 *
 * Revision History:
 *
 * $Log: cdtimemodule.c,v $
 * Revision 1.3  1998/07/09 22:55:25  drach
 * - Added support for GDT absolute time (abstime)
 *
 * Revision 1.2  1998/04/07 17:02:57  drach
 * - Added synonyms
 *
 * Revision 1.1.1.1  1998/01/21 17:55:05  drach
 * Initial repository version
 *
 *
 */

#include "Python.h"             /* Python header files */
#include "cdms.h"
#include "cdtimemodule.h"
#include <math.h>
#include <string.h>
#include <py3c.h>

#define VALCMP(a,b) ((a)<(b)?-1:(b)<(a)?1:0)
#define GET_CALENDAR PyLong_AsLong(PyDict_GetItemString(PyCdtime_ModuleDict, "DefaultCalendar"))
#define SET_CALENDAR(calen) DefineLongConstant(PyCdtime_ModuleDict, "DefaultCalendar", (long)calen)

static PyCdReltimeObject *newreltimeobject(double, char*);
static PyCdComptimeObject *newcomptimeobject(long year, int month, int day,
        int hour, int minute, double second);

PyObject *
PyCdReltime_Compare(PyCdReltimeObject *v, PyObject *w, int op);
PyObject *
PyCdComptime_Compare(PyCdComptimeObject *v, PyObject *w, int op);

static PyObject *comptime_torel(PyCdComptimeObject *self, char *outunits,
        cdCalenType calendar);

#ifdef PYTHON2
static int oldPyCdComptime_Compare(PyCdComptimeObject *v, PyCdComptimeObject *w);
static int oldPyCdReltime_Compare(PyCdReltimeObject *v, PyCdReltimeObject *w);

#endif

extern int cdParseRelunits(cdCalenType timetype, char* relunits, int* unit,
        cdCompTime* base_comptime);
extern int cdValidateTime(cdCalenType timetype, cdCompTime comptime);
extern void cdCompAddMixed(cdCompTime ct, double value, cdCompTime *result);

PyObject *PyCdtime_ErrorObject; /* locally-raised exception */
PyObject *PyCdtime_ModuleDict; /* module dictionary */

#define DefineLongConstant(dict, name,value) { \
    PyObject *tmpx;\
    tmpx = PyLong_FromLong(value);\
    PyDict_SetItemString( dict, name, tmpx);\
    Py_DECREF(tmpx);\
}

/*****************************************************************************
 * Helper functions
 *****************************************************************************/

static int set_double_to_scalar(double *d, PyObject *value) {

    if (PyInt_Check(value)) {
        *d = (double) PyInt_AsLong(value);
        return 0;
    } else if (PyLong_Check(value)) {
        *d = (double) PyLong_AsLong(value);
        return 0;
    } else if (PyFloat_Check(value)) {
        *d = PyFloat_AsDouble(value);
        return 0;
    } else
        onSetError("Value of time component is not a scalar");
}

static int set_long_to_scalar(long *lval, PyObject *value) {

    if (PyInt_Check(value)){
        *lval = (long) PyInt_AsLong(value);
        return 0;
    } else
        onSetError("Value of time component is not a long integer");
}

static int set_int_to_scalar(int *i, PyObject *value) {

    if (PyInt_Check(value)) {
        *i = (int) PyInt_AsLong(value);
        return 0;
    } else
        onSetError("Value of time component is not an integer");
}

static PyObject *
reltime_add(PyCdReltimeObject *self, double value, PyCdtime_Units units,
        cdCalenType calendar) {
    double result, reltime;
    long incr;
    cdCompTime ct, ctmp;

    /* Increment in integer months */
    if (units >= PyCdtime_Months) {

        switch (units) {
        case PyCdtime_Years:
            incr = (long) (12.0 * value);
            break;
        case PyCdtime_Seasons:
            incr = (long) (3.0 * value);
            break;
        case PyCdtime_Months:
            incr = (long) value;
            break;
        default:
            onError("Invalid units indicator")
            ;
        }
        cdRel2Rel(calendar, self->units, self->value, "months", &reltime);
        reltime += (double) incr;
        cdRel2Rel(calendar, "months", reltime, self->units, &result);
        return (PyObject *) newreltimeobject(result, self->units);
    }
    /* Increment in floating-point hours */
    else {
        switch (units) {
        case PyCdtime_Weeks:
            value *= 168.0;
            break;
        case PyCdtime_Days:
            value *= 24.0;
            break;
        case PyCdtime_Hours:
            break;
        case PyCdtime_Minutes:
            value /= 60.0;
            break;
        case PyCdtime_Seconds:
            value /= 3600.0;
            break;
        default:
            onError("Invalid units indicator")
            ;
        }
        if (calendar == cdMixed) {
            cdRel2Comp(calendar, self->units, self->value, &ctmp);
            cdCompAddMixed(ctmp, value, &ct);
            cdComp2Rel(calendar, ct, self->units, &result);
        } else {
            cdRel2Rel(calendar, self->units, self->value, "hours", &reltime);
            reltime += value;
            cdRel2Rel(calendar, "hours", reltime, self->units, &result);
        }
        return (PyObject *) newreltimeobject(result, self->units);
    }
}

static PyObject *
reltime_tocomp(PyCdReltimeObject *self, cdCalenType calendar) {
    cdCompTime compTime;
    int hour, min;
    double sec, fmin;

    cdRel2Comp(calendar, self->units, self->value, &compTime);
    hour = compTime.hour;
    fmin = 60.0 * (compTime.hour - (double) hour);
    min = fmin;
    sec = 60.0 * (fmin - (double) min);
    if (fabs(sec) < 1.0e-11)
        sec = 0.0;

    return (PyObject *) newcomptimeobject((long) compTime.year,
            (int) compTime.month, (int) compTime.day, hour, min, sec);

}

static PyObject *
reltime_torel(PyCdReltimeObject *self, char *outunits, cdCalenType calendar) {

    double outTime;

    cdRel2Rel(calendar, self->units, self->value, outunits, &outTime);

    return (PyObject *) newreltimeobject(outTime, outunits);
}

static PyObject *
reltime_cmp2(PyCdReltimeObject *self, PyObject *other, cdCalenType calendar) {
    int saveCalendar;
    int comparison;
    PyCdReltimeObject *otherReltime;

    /* The compare is done by the module methods, which
     get the value of the calendar from "DefaultCalendar",
     so temporarily override this. */

    saveCalendar = GET_CALENDAR;
    SET_CALENDAR(calendar);

    if (is_comptimeobject(other)) {
        /* Coerce comptime to reltime */
        otherReltime = (PyCdReltimeObject *) comptime_torel(
                (PyCdComptimeObject *) other, self->units, calendar);
    } else {
        otherReltime = newreltimeobject(
                ((PyCdReltimeObject *) other)->value,
                ((PyCdReltimeObject *) other)->units);
    }

    //#define VALCMP(a,b) ((a)<(b)?-1:(b)<(a)?1:0)

    if (PyCdReltime_Compare(self, (PyObject *) otherReltime, Py_EQ) == Py_True) {
        comparison = 0;
        Py_XDECREF(otherReltime);
    } else if (PyCdReltime_Compare(self, (PyObject *) otherReltime,
    Py_LT) == Py_True) {
        comparison = -1;
        Py_XDECREF(otherReltime);
    } else {
        comparison = 1;
    }
    SET_CALENDAR(saveCalendar);
    return Py_BuildValue("i", comparison);

}
#ifdef PYTHON2
static PyObject *
reltime_cmp(PyCdReltimeObject *self, PyObject *other, cdCalenType calendar) {
    int saveCalendar;
    int result;
    PyCdReltimeObject *otherReltime;

    /* The compare is done by the module methods, which
     get the value of the calendar from "DefaultCalendar",
     so temporarily override this. */

    saveCalendar = GET_CALENDAR;
    SET_CALENDAR(calendar);

    if (is_reltimeobject(other)) {
        result = PyCdReltime_Compare(self, other, Py_EQ);
        SET_CALENDAR(saveCalendar);
        return Py_BuildValue("i", result);
    } else if (is_comptimeobject(other)) {
        /* Coerce comptime to reltime */
        otherReltime = (PyCdReltimeObject *) comptime_torel(
                (PyCdComptimeObject *) other, self->units, calendar);
        result = PyCdReltime_Compare(self, (PyObject *)otherReltime, Py_EQ);
        Py_XDECREF(otherReltime);
        SET_CALENDAR(saveCalendar);
        return Py_BuildValue("i", result);
    } else {
        SET_CALENDAR(saveCalendar);
        onError("Cannot compare a time and non-time object");
    }
}
#endif
static PyObject *
comptime_add(PyCdComptimeObject *self, double value, PyCdtime_Units units,
        cdCalenType calendar) {
    long incr;
    cdCompTime ct, ctmp;
    double reltime;
    int hour, min;
    double sec, fmin;

    /* Increment in integer months */
    if (units >= PyCdtime_Months) {

        switch (units) {
        case PyCdtime_Years:
            incr = (long) (12.0 * value);
            break;
        case PyCdtime_Seasons:
            incr = (long) (3.0 * value);
            break;
        case PyCdtime_Months:
            incr = (long) value;
            break;
        default:
            onError("Invalid units indicator")
            ;
        }
        ct.year = self->year;
        ct.month = self->month;
        ct.day = self->day;
        ct.hour = (double) self->hour
                + ((double) self->minute + self->second / 60.0) / 60.0;
        cdComp2Rel(calendar, ct, "months", &reltime);
        reltime += (double) incr;
        cdRel2Comp(calendar, "months", reltime, &ct);
        hour = ct.hour;
        fmin = 60.0 * (ct.hour - (double) hour);
        min = fmin;
        sec = 60.0 * (fmin - (double) min);
        return (PyObject *) newcomptimeobject((long) ct.year, ct.month, ct.day,
                hour, min, sec);
    }
    /* Increment in floating-point hours */
    else {
        switch (units) {
        case PyCdtime_Weeks:
            value *= 168.0;
            break;
        case PyCdtime_Days:
            value *= 24.0;
            break;
        case PyCdtime_Hours:
            break;
        case PyCdtime_Minutes:
            value /= 60.0;
            break;
        case PyCdtime_Seconds:
            value /= 3600.0;
            break;
        default:
            onError("Invalid units indicator")
            ;
        }
        ct.year = self->year;
        ct.month = self->month;
        ct.day = self->day;
        ct.hour = (double) self->hour
                + ((double) self->minute + self->second / 60.0) / 60.0;

        if (calendar == cdMixed) {
            cdCompAddMixed(ct, value, &ctmp);
            ct.year = ctmp.year;
            ct.month = ctmp.month;
            ct.day = ctmp.day;
            ct.hour = ctmp.hour;
        } else {
            cdComp2Rel(calendar, ct, "hours", &reltime);
            reltime += value;
            cdRel2Comp(calendar, "hours", reltime, &ct);
        }
        hour = ct.hour;
        fmin = 60.0 * (ct.hour - (double) hour);
        min = fmin;
        sec = 60.0 * (fmin - (double) min);
        return (PyObject *) newcomptimeobject((long) ct.year, ct.month, ct.day,
                hour, min, sec);
    }
}

static PyObject *
comptime_torel(PyCdComptimeObject *self, char *outunits, cdCalenType calendar) {
    cdCompTime ct;
    double reltime;

    ct.year = self->year;
    ct.month = self->month;
    ct.day = self->day;
    ct.hour = (double) self->hour
            + (((double) self->minute + self->second / 60.0) / 60.0);

    cdComp2Rel(calendar, ct, outunits, &reltime);
    return (PyObject *) newreltimeobject(reltime, outunits);
}

static PyObject *
comptime_tocomp(PyCdComptimeObject *self, cdCalenType calendar) {

    return (PyObject *) newcomptimeobject(self->year, self->month, self->day,
            self->hour, self->minute, self->second);
}
static PyObject *
comptime_cmp2(PyCdComptimeObject *self, PyObject *other, cdCalenType calendar) {
    int saveCalendar;
    int comparison;

    /* The comparisons are done by the module functions, which
     get the calendar from the value of "DefaultCalendar", so
     set this temporarily */
    saveCalendar = GET_CALENDAR;
    SET_CALENDAR(calendar);

    if (PyCdComptime_Compare(self, other, Py_EQ) == Py_True) {
        comparison = 0;
    } else if (PyCdComptime_Compare(self, other, Py_LT) == Py_True) {
        comparison = -1;
    } else {
        comparison = 1;
    }

    SET_CALENDAR(saveCalendar);
    return Py_BuildValue("i", comparison);
}

#ifdef Python2
static PyObject *
comptime_cmp(PyCdComptimeObject *self, PyObject *other, cdCalenType calendar) {
    int saveCalendar;
    PyCdComptimeObject *otherComptime;
    int result;

    /* The comparisons are done by the module functions, which
     get the calendar from the value of "DefaultCalendar", so
     set this temporarily */
    saveCalendar = GET_CALENDAR;
    SET_CALENDAR(calendar);

    if (is_comptimeobject(other)) {
        result = PyCdComptime_Compare(self, (PyCdComptimeObject *) other, Py_EQ);
        SET_CALENDAR(saveCalendar);
        return Py_BuildValue("i", result);
    } else if (is_reltimeobject(other)) {
        /* Coerce reltime to comptime */
        otherComptime = (PyCdComptimeObject *) reltime_tocomp(
                (PyCdReltimeObject *) other, calendar);
        result = PyCdComptime_Compare(self, otherComptime, Py_EQ);
        Py_XDECREF(otherComptime);
        SET_CALENDAR(saveCalendar);
        return Py_BuildValue("i", result);
    } else {
        SET_CALENDAR(saveCalendar);
        onError("Cannot compare a time and non-time object");
    }
}
#endif

/*****************************************************************************
 * INSTANCE METHODS - Relative time
 *****************************************************************************/

/* Add a time increment to a relative time,
 e.g., rnew = r.add(3, Months [, calendar]) */

static PyObject *
PyCdReltime_Add(PyCdReltimeObject *self, PyObject *args) {
    double value;
    PyCdtime_Units units;
    cdCalenType calendar;

    calendar = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "di|i", &value, &units, &calendar))
        return NULL;

    return reltime_add(self, value, units, calendar);
}

/* Subtract a time increment from a relative time
 e.g., rnew = r.sub(3, Months [, calendar]) */

static PyObject *
PyCdReltime_Sub(PyCdReltimeObject *self, PyObject *args) {
    double value;
    PyCdtime_Units units;
    cdCalenType calendar;

    calendar = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "di|i", &value, &units, &calendar))
        return NULL;

    return reltime_add(self, -value, units, calendar);
}

/* Convert a relative time to component time
 e.g., r.tocomp([calendar])*/

static PyObject *
PyCdReltime_Tocomp(PyCdReltimeObject *self, PyObject *args) {
    cdCalenType calentype;

    calentype = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "|i", &calentype))
        return NULL;

    return reltime_tocomp(self, calentype);
}

/* Convert a relative time to a relative time
 with different units, e.g.,
 rnew = r.torel("days since 1997" [, calendar])*/

static PyObject *
PyCdReltime_Torel(PyCdReltimeObject *self, PyObject *args) {
    cdCalenType calentype;
    char *outunits;

    calentype = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "s|i", &outunits, &calentype))
        return NULL;

    return reltime_torel(self, outunits, calentype);
}

/* Return -1, 0, or 1 as self is less than, equal to,
 or greater than other. Syntax is r.cmp(other [,calendar])
 other may be a reltime or a comptime. */
static PyObject *
PyCdReltime_Cmp(PyCdReltimeObject *self, PyObject *args) {
    int calendar;
    PyObject *other;

    calendar = GET_CALENDAR;

    if (!PyArg_ParseTuple(args, "O|i", &other, &calendar))
        return NULL;

    return reltime_cmp2(self, other, calendar);
}

static struct PyMethodDef reltime_instance_methods[] = { /* instance methods */
{ "add", (PyCFunction) PyCdReltime_Add, 1 }, { "sub",
        (PyCFunction) PyCdReltime_Sub, 1 }, { "tocomp",
        (PyCFunction) PyCdReltime_Tocomp, 1 }, { "tocomponent",
        (PyCFunction) PyCdReltime_Tocomp, 1 }, { "torel",
        (PyCFunction) PyCdReltime_Torel, 1 }, { "torelative",
        (PyCFunction) PyCdReltime_Torel, 1 }, { "cmp",
        (PyCFunction) PyCdReltime_Cmp, 1 }, { NULL, NULL } };

/*****************************************************************************
 * INSTANCE METHODS - Component time
 *****************************************************************************/

/* Add a time increment to a component time,
 e.g., cnew = c.add(3, Months [, calendar]) */

static PyObject *
PyCdComptime_Add(PyCdComptimeObject *self, PyObject *args) {
    double value;
    PyCdtime_Units units;
    cdCalenType calendar;

    calendar = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "di|i", &value, &units, &calendar))
        return NULL;

    return comptime_add(self, value, units, calendar);
}

/* Subtract a time increment from a component time
 e.g., cnew = c.sub(3, Months [, calendar]) */

static PyObject *
PyCdComptime_Sub(PyCdComptimeObject *self, PyObject *args) {
    double value;
    PyCdtime_Units units;
    cdCalenType calendar;

    calendar = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "di|i", &value, &units, &calendar))
        return NULL;

    return comptime_add(self, -value, units, calendar);
}

/* Convert a component time to a relative time, e.g.,
 rnew = c.torel("days since 1997" [, calendar])*/

static PyObject *
PyCdComptime_Torel(PyCdComptimeObject *self, PyObject *args) {
    cdCalenType calentype;
    char *outunits;

    calentype = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "s|i", &outunits, &calentype))
        return NULL;

    return comptime_torel(self, outunits, calentype);
}

/* Copy a component time. This function is provided
 so that tocomp() can be used for both types.
 cnew = c.torel([calendar])*/

static PyObject *
PyCdComptime_Tocomp(PyCdComptimeObject *self, PyObject *args) {
    cdCalenType calentype;

    calentype = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "|i", &calentype))
        return NULL;

    return comptime_tocomp(self, calentype);
}

static PyObject *
PyCdComptime_Cmp(PyCdComptimeObject *self, PyObject *args) {
    int calendar;
    PyObject *other;

    calendar = GET_CALENDAR;

    if (!PyArg_ParseTuple(args, "O|i", &other, &calendar))
        return NULL;

    return comptime_cmp2(self, other, calendar);

}

static struct PyMethodDef comptime_instance_methods[] = { /* instance methods */
{ "add", (PyCFunction) PyCdComptime_Add, 1 }, { "sub",
        (PyCFunction) PyCdComptime_Sub, 1 }, { "torel",
        (PyCFunction) PyCdComptime_Torel, 1 }, { "torelative",
        (PyCFunction) PyCdComptime_Torel, 1 }, { "tocomp",
        (PyCFunction) PyCdComptime_Tocomp, 1 }, { "tocomponent",
        (PyCFunction) PyCdComptime_Tocomp, 1 }, { "cmp",
        (PyCFunction) PyCdComptime_Cmp, 1 }, { NULL, NULL } };

/*****************************************************************************
 * BASIC TYPE-OPERATIONS - Relative time
 *****************************************************************************/

static PyCdReltimeObject * /* on "x = cdtime.reltime()" */
newreltimeobject(double value, char *units) /* instance constructor function */
{ /* these don't get an 'args' input */
    PyCdReltimeObject *self;
    cdCalenType calendar;
    int relunits;
    cdCompTime basetime;

    calendar = GET_CALENDAR;

    /* Check that the relative time units will parse */
    if (cdParseRelunits(calendar, units, &relunits, &basetime)){
        onError("Invalid relative time units");
    }
    self = PyObject_NEW(PyCdReltimeObject, &RelTimeType); /* malloc, init, incref */
    if (self == NULL)
        return NULL; /* raise exception */
    self->value = value;
    strncpy(self->units, units, CD_MAX_RELUNITS);
    self->units[CD_MAX_RELUNITS] = '\0';
    return self; /* a new type-instance object */
}

static void /* instance destructor function */
PyCdReltime_Dealloc(PyCdReltimeObject *self) /* when reference-count reaches zero */
{ /* do cleanup activity */
    PyObject_Del(self); /* PyMem_Del segfaults in 2.5 */
}

static int PyCdReltime_Print(PyCdReltimeObject *self, FILE *fp, int flags) { /* or repr or str */
    fprintf(fp, "%.2lf %s", self->value, self->units);
    return 0; /* return status, not object */
}

static PyObject *
PyCdReltime_Repr(PyCdReltimeObject *self) {
    char buf[CD_MAX_RELUNITS + 20];
    sprintf(buf, "%.6lf %s", self->value, self->units);
    return Py_BuildValue("s", buf);
}

static PyObject *
PyCdReltime_Getattro(PyCdReltimeObject *self, PyObject *nameobj) /* on "instance.attr" reference  */
{ /* exposed data-members */
    char *objname;
    objname = PyStr_AsString(nameobj);
    if (strcmp(objname, "value") == 0)
        return Py_BuildValue("d", self->value);
    else if (strcmp(objname, "units") == 0)
        return Py_BuildValue("s", self->units);
    else if (strcmp(objname, "__members__") == 0) /* __methods__ is free */
        return Py_BuildValue("[ss]", "value", "units"); /* make a list of 1 string */
    else
        return PyObject_GenericGetAttr((PyObject *) self, nameobj);
}

/* on instance.attr = value */
static int PyCdReltime_Setattro(PyCdReltimeObject *self, char *name,
        PyObject *value) {

    if (strcmp(name, "value") == 0) {
        return set_double_to_scalar(&self->value, value);
    } else if (strcmp(name, "units") == 0) {
        if (PyStr_Check(value)) {
            strncpy(self->units, PyStr_AsUTF8(value), CD_MAX_RELUNITS);
            self->units[CD_MAX_RELUNITS] = '\0';
            return 0;
        } else
            onSetError("Relative time units must be a string");
    } else
        onSetError("Relative time attribute unknown or read-only");
}

PyObject *
PyCdReltime_Compare(PyCdReltimeObject *v, PyObject *other, int op) {
    cdCalenType calendar;
    double s, o;
    PyObject *r;
    PyCdReltimeObject *w;
    calendar = GET_CALENDAR;
    w = (PyCdReltimeObject *) other;
    if(is_comptimeobject(other)) {
        /* Coerce comptime to reltime */
        w = (PyCdReltimeObject *) comptime_torel(
                (PyCdComptimeObject *) other, v->units, calendar);
    } else {
        w = newreltimeobject(
                ((PyCdReltimeObject *)other)->value,
                ((PyCdReltimeObject *)other)->units);
    }
    cdRel2Rel(calendar, v->units, v->value, "days", &s);
    cdRel2Rel(calendar, w->units, w->value, "days", &o);
    Py_XDECREF(w);
    r=Py_False;
    switch (op) {
    case Py_LT:
        if (s < o) {
            r = Py_True;
        }
        break;
    case Py_LE:
        if (s <= o) {
            r = Py_True;
        }
        break;
    case Py_EQ:
        if (s == o) {
            r = Py_True;
        }
        break;
    case Py_NE:
        if (s != o) {
            r = Py_True;
        }
        break;
    case Py_GT:
        if (s > o) {
            r = Py_True;
        }
        break;
    case Py_GE:
        if (s >= o) {
            r = Py_True;
        }
        break;
    }
    Py_INCREF(r);
    return(r);
}
/*****************************************************************************
 * BASIC TYPE-OPERATIONS - Component time
 *****************************************************************************/

static PyCdComptimeObject * /* on "x = cdtime.comptime()" */
newcomptimeobject(long year, int month, int day, int hour, int minute,
        double second) /* instance constructor function */
{ /* these don't get an 'args' input */
    PyCdComptimeObject *self;
    cdCompTime ct;
    char *absunits;
    double absvalue;

    absunits = "day as %Y%m%d.%f";
    if (second < 0.0)
        second = 0.0;
    if (minute < 0)
        minute = 0;
    if (hour < 0) {
        hour = 0;
        absunits = "day as %Y%m%d.%f";
    }
    if (day <= 0) {
        day = 1;
        absunits = "calendar_month as %Y%m.%f";
    }
    if (month <= 0) {
        month = 1;
        absunits = "calendar_year as %Y.%f";
    }

    ct.year = year;
    ct.month = month;
    ct.day = day;
    ct.hour = (double) hour + ((double) minute + second / 60.0) / 60.0;
    if (cdValidateTime(cdStandard, ct)) /* calendar is ignored */
        onError("Invalid component time");

    self = PyObject_NEW(PyCdComptimeObject, &CompTimeType); /* malloc, init, incref */
    if (self == NULL)
        return NULL; /* raise exception */
    self->year = year;
    self->month = month;
    self->day = day;
    self->hour = hour;
    self->minute = minute;
    self->second = second;

    if (cdComp2Abs(ct, absunits, cdDouble, 0.0, &absvalue))
        onError("Invalid component time");

    self->absvalue = absvalue;
    strncpy(self->absunits, absunits, CD_MAX_ABSUNITS);
    self->fraction = 0.0;
    return self; /* a new type-instance object */
}

static PyCdComptimeObject * /* on "x = cdtime.abstime()" */
newabstimeobject(long year, int month, int day, int hour, int minute,
        double second, double absvalue, char *absunits, double fraction) /* instance constructor function */
{ /* these don't get an 'args' input */
    PyCdComptimeObject *self;

    self = newcomptimeobject(year, month, day, hour, minute, second);
    if (self == NULL)
        return NULL;
    self->absvalue = absvalue;
    strncpy(self->absunits, absunits, CD_MAX_ABSUNITS);
    self->fraction = fraction;
    return self; /* a new type-instance object */
}

static void /* instance destructor function */
PyCdComptime_Dealloc(PyCdComptimeObject *self) /* when reference-count reaches zero */
{ /* do cleanup activity */
    PyObject_Del(self); /* PyMem_Del segfaults in 2.5 */
}

static int PyCdComptime_Print(PyCdComptimeObject *self, FILE *fp, int flags) { /* or repr or str */
    fprintf(fp, "%ld-%d-%d %d:%d:%.1lf", self->year, self->month, self->day,
            self->hour, self->minute, self->second);
    return 0; /* return status, not object */
}

static PyObject *
PyCdComptime_Repr(PyCdComptimeObject *self) {
    char buf[128];
    sprintf(buf, "%ld-%d-%d %d:%d:%.1lf", self->year, self->month, self->day,
            self->hour, self->minute, self->second);
    return Py_BuildValue("s", buf);
}

static PyObject *
PyCdComptime_Getattro(PyCdComptimeObject *self, PyObject *nameobj) /* on "instance.attr" reference  */
{ /* exposed data-members */
    char *objname;
    objname = PyStr_AsString(nameobj);
    if (strcmp(objname, "year") == 0) /* really C struct fields */
        return Py_BuildValue("l", self->year);
    else if (strcmp(objname, "month") == 0)
        return Py_BuildValue("i", self->month);
    else if (strcmp(objname, "day") == 0)
        return Py_BuildValue("i", self->day);
    else if (strcmp(objname, "hour") == 0)
        return Py_BuildValue("i", self->hour);
    else if (strcmp(objname, "minute") == 0)
        return Py_BuildValue("i", self->minute);
    else if (strcmp(objname, "second") == 0)
        return Py_BuildValue("d", self->second);
    else if (strcmp(objname, "__members__") == 0) /* __methods__ is free */
        return Py_BuildValue("[sssssssss]", "year", "month", "day", "hour",
                "minute", "second", "absvalue", "absunits", "fraction"); /* make a list of 1 string */
    else if (strcmp(objname, "absvalue") == 0)
        return Py_BuildValue("d", self->absvalue);
    else if (strcmp(objname, "absunits") == 0)
        return Py_BuildValue("s", self->absunits);
    else if (strcmp(objname, "fraction") == 0)
        return Py_BuildValue("d", self->fraction);
    else
        return PyObject_GenericGetAttr((PyObject *) self, nameobj);
}

/* on instance.attr = value */
static int PyCdComptime_Setattro(PyCdComptimeObject *self, PyObject *name,
        PyObject *value) {

    char *objname;
    objname = PyStr_AsString(name);
    if (strcmp(objname, "year") == 0)
        return set_long_to_scalar(&self->year, value);
    else if (strcmp(objname, "month") == 0)
        return set_int_to_scalar(&self->month, value);
    else if (strcmp(objname, "day") == 0)
        return set_int_to_scalar(&self->day, value);
    else if (strcmp(objname, "hour") == 0)
        return set_int_to_scalar(&self->hour, value);
    else if (strcmp(objname, "minute") == 0)
        return set_int_to_scalar(&self->minute, value);
    else if (strcmp(objname, "second") == 0)
        return set_double_to_scalar(&self->second, value);
    else
        onSetError("Component time attribute unknown or read-only");
}

PyObject *
PyCdComptime_Compare(PyCdComptimeObject *s, PyObject *other, int op) {
    cdCalenType calendar;
    PyObject *r;
    calendar = GET_CALENDAR;
    r = Py_False;
    PyCdComptimeObject *o;
    o = (PyCdComptimeObject*) other;
    long sValue;
    long oValue;

    if(is_reltimeobject(other)) {
        /* Coerce comptime to reltime */
        o = (PyCdComptimeObject *) reltime_tocomp(
                (PyCdReltimeObject *) other, calendar);
    }
    sValue = s->year * 1e10 + s->month * 1e8 + s->day * 1e6 + s->hour * 1e4 +
            s->minute * 1e2 + s->second;
    oValue = o->year * 1e10 + o->month * 1e8 + o->day * 1e6 + o->hour * 1e4 +
            o->minute * 1e2 + o->second;

    switch (op) {
    case Py_LT:
        if (sValue < oValue) {
            r = Py_True;
        }
        break;
    case Py_LE:
        if (sValue <= oValue) {
            r = Py_True;
        }
        break;
    case Py_EQ:
        if (sValue == oValue) {
            r = Py_True;
        }
        break;
    case Py_NE:
        if (sValue != oValue) {
            r = Py_True;
        }
        break;
    case Py_GT:
        if (sValue > oValue) {
            r = Py_True;
        }
        break;
    case Py_GE:
        if (sValue >= oValue) {
            r = Py_True;
        }
        break;
    }
    Py_INCREF(r);
    return r;
}
#ifdef PYTHON2
static int oldPyCdReltime_Compare(PyCdReltimeObject *v, PyCdReltimeObject *w) {
    cdCalenType calendar;
    double v_days, w_days;

    calendar = GET_CALENDAR;
    /* Convert both values to "days", then compare */
    cdRel2Rel(calendar, v->units, v->value, "days", &v_days);
    cdRel2Rel(calendar, w->units, w->value, "days", &w_days);
    return VALCMP(v_days, w_days);
}

static int oldPyCdComptime_Compare(PyCdComptimeObject *v, PyCdComptimeObject *w) {

    if (VALCMP(v->year, w->year))
        return(VALCMP(v->year, w->year));
    else if (VALCMP(v->month, w->month))
        return(VALCMP(v->month, w->month));
    else if (VALCMP(v->day, w->day))
        return(VALCMP(v->day, w->day));
    else if (VALCMP(v->hour, w->hour))
        return(VALCMP(v->hour, w->hour));
    else if (VALCMP(v->minute, w->minute))
        return(VALCMP(v->minute, w->minute));
    else
        return(VALCMP(v->second, w->second));

}
#endif

/*****************************************************************************
 * TYPE DESCRIPTORS
 *****************************************************************************/

static PyTypeObject RelTimeType = { /* main python type-descriptor */
/* type header *//* shared by all instances */
PyVarObject_HEAD_INIT(NULL, 0)
"reltime", /* tp_name */
sizeof(PyCdReltimeObject), /* tp_basicsize */
0, /* tp_itemsize */

/* standard methods */
(destructor) PyCdReltime_Dealloc, /* tp_dealloc  ref-count==0  */
(printfunc) PyCdReltime_Print, /* tp_print    "print x"     */
0, /* tp_getattr  "x.attr"      */
0, /* tp_setattr  "x.attr=v"    */
0, /* tp_compare  "x > y"       */
(reprfunc) PyCdReltime_Repr, /* tp_repr     `x`, print x  */

/* type categories */
0, /* tp_as_number   +,-,*,/,%,&,>>,pow...*/
0, /* tp_as_sequence +,[i],[i:j],len, ...*/
0, /* tp_as_mapping  [key], len, ...*/

/* more methods */
0, /* tp_hash    "dict[x]" */
0, /* tp_call    "x()"     */
0, /* tp_str     "str(x)"  */
(getattrofunc) PyCdReltime_Getattro, /* tp_getattro */
(setattrofunc) PyCdReltime_Setattro, /* tp_setattro */
0, /* tp_as_buffer */
0, /* tp_flags */
0, /* tp_doc */
0, /* tp_traverse */
0, /* tp_clear */
(richcmpfunc) PyCdReltime_Compare, /* tp_richcompare */
0, /* tp_weaklistoffset */
0, /* tp_iter */
0, /* tp_iternext */
(PyMethodDef*) reltime_instance_methods /* tp_methods */
}; /* plus others: see Include/object.h */

static PyTypeObject CompTimeType = { /* main python type-descriptor */
/* type header *//* shared by all instances */
PyVarObject_HEAD_INIT(NULL,0)
"comptime", /* tp_name */
sizeof(PyCdComptimeObject), /* tp_basicsize */
0, /* tp_itemsize */

/* standard methods */
(destructor) PyCdComptime_Dealloc, /* tp_dealloc  ref-count==0  */
(printfunc) PyCdComptime_Print, /* tp_print    "print x"     */
0, /* tp_getattr  "x.attr"      */
0, /* tp_setattr  "x.attr=v"    */
0, /* tp_compare  "x > y"       */
(reprfunc) PyCdComptime_Repr, /* tp_repr     `x`, print x  */

/* type categories */
0, /* tp_as_number   +,-,*,/,%,&,>>,pow...*/
0, /* tp_as_sequence +,[i],[i:j],len, ...*/
0, /* tp_as_mapping  [key], len, ...*/

/* more methods */
0, /* tp_hash    "dict[x]" */
0, /* tp_call    "x()"     */
0, /* tp_str     "str(x)"  */
(getattrofunc) PyCdComptime_Getattro, /* tp_getattro */
(setattrofunc) PyCdComptime_Setattro, /* tp_setattro */
0, /* tp_as_buffer */
0, /* tp_flags */
0, /* tp_doc */
0, /* tp_traverse */
0, /* tp_clear */
(richcmpfunc) PyCdComptime_Compare, /* tp_richcompare */
0, /* tp_weaklistoffset */
0, /* tp_iter */
0, /* tp_iternext */
(PyMethodDef*) comptime_instance_methods /* tp_methods */
}; /* plus others: see Include/object.h */

/******************************************************************************
 * EXPORTED MODULE METHOD-FUNCTIONS
 ******************************************************************************/

/* Character string -> component time */

static char doc_char2comp[] = "s2c(string [,calendar])";

static PyObject *
PyCdtime_Char2Comp(PyObject *self, PyObject *args) {
    int calentype = cdMixed;
    char *ctime;
    cdCompTime compTime;
    int hour, min;
    double sec, fmin;

    if (!PyArg_ParseTuple(args, "s|i", &ctime, &calentype))
        return NULL;
    cdChar2Comp(calentype, ctime, &compTime);
    hour = compTime.hour;
    fmin = 60.0 * (compTime.hour - (double) hour);
    min = fmin;
    sec = 60.0 * (fmin - (double) min);

    return (PyObject *) newcomptimeobject((long) compTime.year,
            (int) compTime.month, (int) compTime.day, hour, min, sec);
}

/* Character string -> relative time */

static char doc_char2rel[] = "s2r(string, relunits [, calendar])";

static PyObject *
PyCdtime_Char2Rel(PyObject *self, PyObject *args) {
    int calentype = cdMixed;
    char *ctime, *runits;
    double rtime;

    if (!PyArg_ParseTuple(args, "ss|i", &ctime, &runits, &calentype))
        return NULL;
    cdChar2Rel(calentype, ctime, runits, &rtime);

    return (PyObject *) newreltimeobject(rtime, runits);
}

/* Component time -> relative time */

static char doc_comp2rel[] = "c2r( comptime, relunits [,calendar])";

static PyObject *
PyCdtime_Comp2Rel(PyObject *self, PyObject *args) {
    int calentype;
    char *relUnits;
    PyCdComptimeObject *pyCompTime;

    calentype = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "Os|i", &pyCompTime, &relUnits, &calentype))
        return NULL;

    return comptime_torel(pyCompTime, relUnits, calentype);
}

/* Relative time -> component time */

static char doc_rel2comp[] = "r2c( reltime [, calendar])";

static PyObject *
PyCdtime_Rel2Comp(PyObject *self, PyObject *args) {
    int calentype;
    PyCdReltimeObject *pyRelTime;

    calentype = GET_CALENDAR;

    if (!PyArg_ParseTuple(args, "O|i", &pyRelTime, &calentype))
        return NULL;

    return reltime_tocomp(pyRelTime, calentype);
}

/* Relative time -> relative time */

static char doc_rel2rel[] = "r2r( reltime, newunits [, calendar])";

static PyObject *
PyCdtime_Rel2Rel(PyObject *self, PyObject *args) {
    int calentype;
    char *outUnits;
    PyCdReltimeObject *pyRelTime;

    calentype = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "Os|i", &pyRelTime, &outUnits, &calentype))
        return NULL;
    return reltime_torel(pyRelTime, outUnits, calentype);
}

static char doc_cmp[] = "cmp( time, time [, calendar])";

static PyObject *
PyCdtime_Compare(PyObject *self, PyObject *args) { /* on "x = cdtime.cmp()" */
    int calendar;
    PyObject *t1, *t2;

    calendar = GET_CALENDAR;
    if (!PyArg_ParseTuple(args, "OO|i", &t1, &t2, &calendar))
        return NULL;

    if (is_reltimeobject(t1)) {
        return reltime_cmp2((PyCdReltimeObject *) t1, t2, calendar);
    } else if (is_comptimeobject(t1)) {
        return comptime_cmp2((PyCdComptimeObject *) t1, t2, calendar);
    } else
        onError("Argument is not a time");
}

static PyObject *
PyCdtime_ReltimeNew(PyObject *self, PyObject *args) /* on "x = cdtime.reltime()" */
{
    double value;
    char *units;

    if (!PyArg_ParseTuple(args, "ds", &value, &units)) /* Module-method function */
        return NULL;
    return (PyObject *) newreltimeobject(value, units); /* make a new type-instance object */
} /* the hook from module to type... */

static PyObject *
PyCdtime_ComptimeNew(PyObject *self, PyObject *args) /* on "x = cdtime.comptime()" */
{
    long year;
    int month = 0;
    int day = 0;
    int hour = -1;
    int minute = -1;
    double second = -1.0;

    if (!PyArg_ParseTuple(args, "l|iiiid", &year, &month, &day, &hour, &minute,
            &second)) /* Module-method function */
        return NULL;
    return (PyObject *) newcomptimeobject(year, month, day, hour, minute,
            second); /* make a new type-instance object */
}

static PyObject *
PyCdtime_AbstimeNew(PyObject *self, PyObject *args) /* on "x = cdtime.abstime()" */
{
    double absvalue;
    char *absunits;
    cdCompTime comptime;
    double fraction;
    int hour, minute;
    double second, fmin;

    if (!PyArg_ParseTuple(args, "ds", &absvalue, &absunits)) /* Module-method function */
        return NULL;

    if (cdAbs2Comp(absunits, (void *) &absvalue, cdDouble, &comptime,
            &fraction))
        onError("Invalid absolute time");

    hour = comptime.hour;
    fmin = 60.0 * (comptime.hour - (double) hour);
    minute = fmin;
    second = 60.0 * (fmin - (double) minute);

    return (PyObject *) newabstimeobject((long) comptime.year, comptime.month,
            comptime.day, hour, minute, second, absvalue, absunits, fraction); /* make a new type-instance object */

}

/* the hook from module to type... */

/******************************************************************************
 * METHOD REGISTRATION TABLE: NAME-STRING -> FUNCTION-POINTER
 ******************************************************************************/

static struct PyMethodDef cdtime_methods[] = { { "reltime", PyCdtime_ReltimeNew,
        1 }, /* make a reltime */
{ "relativetime", PyCdtime_ReltimeNew, 1 }, /* make a reltime */
{ "comptime", PyCdtime_ComptimeNew, 1 }, /* make a comptime */
{ "abstime", PyCdtime_AbstimeNew, 1 }, /* make a comptime from an absolute time */
{ "componenttime", PyCdtime_ComptimeNew, 1 }, /* make a comptime */
{ "s2c", PyCdtime_Char2Comp, 1, doc_char2comp }, /* Character to component */
{ "s2r", PyCdtime_Char2Rel, 1, doc_char2rel }, /* Character to relative */
{ "c2r", PyCdtime_Comp2Rel, 1, doc_comp2rel }, /* Component to relative */
{ "r2c", PyCdtime_Rel2Comp, 1, doc_rel2comp }, /* Relative to component */
{ "r2r", PyCdtime_Rel2Rel, 1, doc_rel2rel }, /* Relative to relative */
{ "compare", PyCdtime_Compare, 1, doc_cmp }, /* Compare two times */
{ NULL, NULL } /* end, for initmodule */
};

/******************************************************************************
 * MODULE INITIALIZATION FUNCTION
 ******************************************************************************/

static struct PyModuleDef moduledef = {
PyModuleDef_HEAD_INIT, "cdtime", /* m_name */
"", /* m_doc */
-1, /* m_size */
cdtime_methods /* m_methods */
};
MODULE_INIT_FUNC(cdtime) {
    PyObject *m, *d;

    /* Initialize type object headers */
    Py_TYPE(&RelTimeType) = &PyType_Type;
    Py_TYPE(&CompTimeType) = &PyType_Type;

    /* create the module and add the functions */
    m = PyModule_Create(&moduledef); /* registration hook */

    if(m == NULL) {
        return(NULL);
    }
    /* add symbolic constants to the module */
    d = PyCdtime_ModuleDict = PyModule_GetDict(m);
    PyCdtime_ErrorObject = PyExc_ValueError;
    PyDict_SetItemString(d, "error", PyCdtime_ErrorObject); /* export exception */
    DefineLongConstant(d, "StandardCalendar", (long)cdStandard);
    DefineLongConstant(d, "GregorianCalendar", (long)cdStandard);
    DefineLongConstant(d, "JulianCalendar", (long)cdJulian);
    DefineLongConstant(d, "MixedCalendar", (long)cdMixed);
    DefineLongConstant(d, "NoLeapCalendar", (long)cdNoLeap);
    DefineLongConstant(d, "Calendar360", (long)cd360);
    DefineLongConstant(d, "ClimCalendar", (long)cdClim);
    DefineLongConstant(d, "ClimLeapCalendar", (long)cdClimLeap);
    DefineLongConstant(d, "DefaultCalendar", (long)cdMixed); /* initialize the default calendar */

    DefineLongConstant(d, "Seconds", (long)PyCdtime_Seconds);
    DefineLongConstant(d, "Second", (long)PyCdtime_Seconds);
    DefineLongConstant(d, "Minutes", (long)PyCdtime_Minutes);
    DefineLongConstant(d, "Minute", (long)PyCdtime_Minutes);
    DefineLongConstant(d, "Hours", (long)PyCdtime_Hours);
    DefineLongConstant(d, "Hour", (long)PyCdtime_Hours);
    DefineLongConstant(d, "Days", (long)PyCdtime_Days);
    DefineLongConstant(d, "Day", (long)PyCdtime_Days);
    DefineLongConstant(d, "Weeks", (long)PyCdtime_Weeks);
    DefineLongConstant(d, "Week", (long)PyCdtime_Weeks);
    DefineLongConstant(d, "Months", (long)PyCdtime_Months);
    DefineLongConstant(d, "Month", (long)PyCdtime_Months);
    DefineLongConstant(d, "Seasons", (long)PyCdtime_Seasons);
    DefineLongConstant(d, "Season", (long)PyCdtime_Seasons);
    DefineLongConstant(d, "Years", (long)PyCdtime_Years);
    DefineLongConstant(d, "Year", (long)PyCdtime_Years);

    /* check for errors */
    if (PyErr_Occurred())
    Py_FatalError("can't initialize module cdtime");
    return m;
}

