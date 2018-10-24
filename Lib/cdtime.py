
import _cdtime

error = _cdtime.error
StandardCalendar = _cdtime.StandardCalendar
GregorianCalendar = _cdtime.GregorianCalendar
JulianCalendar = _cdtime.JulianCalendar
MixedCalendar = _cdtime.MixedCalendar
NoLeapCalendar = _cdtime.NoLeapCalendar
Calendar360 = _cdtime.Calendar360
ClimCalendar = _cdtime.ClimCalendar
ClimLeapCalendar = _cdtime.ClimLeapCalendar
DefaultCalendar = _cdtime.DefaultCalendar
Second = _cdtime.Second
Minutes = _cdtime.Minutes
Minute = _cdtime.Minute
Hour = _cdtime.Hour
Days = _cdtime.Days
Day = _cdtime.Day
Weeks = _cdtime.Weeks
Week = _cdtime.Week
Months = _cdtime.Months
Month = _cdtime.Month
Seasons = _cdtime.Seasons
Season = _cdtime.Season
Years = _cdtime.Years
Year = _cdtime.Year


def abstime(value, units):
    return _cdtime.abstime(value, units)


def c2r(ctype, caltype=0):
    """
    c2r(comptime, relunits[, calendar])
    """
    return _cdtime.c2r(ctype, caltype)


def compare(cdtime1, cdtime2, calendar=_cdtime.DefaultCalendar):
    "cmp( time, time [, calendar])"
    return _cdtime.compare(cdtime1, cdtime2, calendar)


def componenttime(year, month=1, day=1, hour=0, minute=0, second=0):
    return _cdtime.componenttime(year, month, day, hour, minute, second)


def comptime(year, month=1, day=1, hour=0, minute=0, second=0):
    return _cdtime.comptime(year, month, day, hour, minute, second)


def r2c(reltime, calendar=None):
    "r2c( reltime [, calendar])"
    return _cdtime.r2c(reltime, calendar)


def r2r(reltime, newunits, calendar=_cdtime.DefaultCalendar):
    "r2r( reltime, newunits [, calendar])"
    return _cdtime.r2r(reltime, newunits, calendar=None)


def relativetime(value, units):
    return _cdtime.relativetime(value, units)


def reltime(value, units):
    return _cdtime.reltime(value, units)


def s2c(stirng, calendar=_cdtime.DefaultCalendar):
    """
    s2c(string[, calendar])
    """
    return _cdtime.s2c(string, calendar)


def s2r(ctime, runits, calendar=_cdtime.DefaultCalendar):
    """
    s2r(string, relunits [, calendar])
    """
    return _cdtime.s2r(ctime, runits, calendar)


def __getstate__():
    """Return the internal state of the masked array.
    This is for pickling.
    """
    # import pdb
    # pdb.set_trace()
    # state = (1,
    #          .value
    #          .units
    #          )
    return None


# def __setstate__(state):
#     """
#     Restore the internal state of the masked array.
#     This is for pickling.  ``state`` is typically the output of the
#     ``__getstate__`` output, and is a 5-tuple:
#     - class name
#     - a tuple giving the shape of the data
#     - a typecode for the data
#     - a binary string for the data
#     - a binary string for the mask.
#     """
#     import pdb
#     pdb.set_trace()
#     (ver, value, units) = state
#     cdtime.reltime(value, units)
# #    ndarray.__setstate__( (shp, typ, isf, raw))
# #    mdtype = dtype([(k, bool_) for (k, _) in .dtype.descr])
# #    .__dict__['_mask'].__setstate__((shp, mdtype, isf, msk))
#     # .fill_value = flv

# # return (_mareconstruct, (.__class__, ._baseclass, (0,), 'b',), .__getstate__())


# def __reduce__():
#     """
#     Return a 3-tuple for pickling a MaskedArray.
#     """
#     import pdb
#     pdb.set_trace()
#     return (_.reltime, (.__class__, .value, .units), .__getstate__())


# def _mrreconstruct(subtype, baseclass, baseshape, basetype,):
#     """
#     Build a new MaskedArray from the information stored in a pickle.
#     """
#     _data = ndarray.__new__(baseclass, baseshape, basetype).view(subtype)
#     _mask = ndarray.__new__(ndarray, baseshape, 'b1')
#     return subtype.__new__(subtype, _data, mask=_mask, dtype=basetype,)
