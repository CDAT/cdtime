
from . import _cdtime

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


def c2r(ctype, newunits):
    """
        c2r(comptime, relunits[, calendar])

        Parameters
        ----------

            ctype:  component time class object
            newunits: [seconds,hours,days,yeears] since [year]

        Returns
        -------

           Relative time class object.
        ::

            >>> myComponentTime = cdtime.comptime(2020,01,10)
            >>> myRelativeTime = cdtime.c2r(myComponentTime,"days since 2020")
            >>> myRelativeTime
            9.00 days since 2020

    """
    return _cdtime.c2r(ctype, newunits)


def compare(cdtime1, cdtime2, calendar=_cdtime.DefaultCalendar):
    """
        compare time object

            cmp(time1, time2[, calendar])

        Parameters
        ----------

            time1:  component or relative time class object
            time2:  component or relative time class object
            calendar: [default to Gregorian]

        Returns
        -------

           -1:  if time1 < time2
            0: if time == time2
            1: if time1 > time2
        ::

            >>> myComponentTime = cdtime.comptime(2020,01,10)
            >>> myRelativeTime = cdtime.relativetime(11 "days since 2020")
            >>> cdtime.cmp(myComponentTime, myRelativeTime, cdtime.GregorianCalendar)
            >>> cdtime.compare(myRelativeTime, myComponentTime, cdtime.GregorianCalendar)
            1
            >>> cdtime.compare(myComponentTime, myRelativeTime, cdtime.GregorianCalendar)
            -1

    """
    return _cdtime.compare(cdtime1, cdtime2, calendar)


def componenttime(year, month=1, day=1, hour=0, minute=0, second=0):
    """
        Create a component time object.

            componenttime(year, month, day, hour, minute, second)

        Parameters
        ----------
            year:
            month:  default = 1
            day: default = 1
            hour: default = 0
            minute: default = 0
            second: default = 0

        Returns
        -------
            Component Time class object

        ::
            >>> timeStart = cdtime.comptime(2017)
            >>> timeEnd = cdtime.comptime(2017,12,31,23,59,59)
            >>> timeStart
            2017-1-1 0:0:0.0
            >>> timeEnd
            2017-12-31 23:59:59.0

    """
    return _cdtime.componenttime(year, month, day, hour, minute, second)


def comptime(year, month=1, day=1, hour=0, minute=0, second=0):
    """
        Create a component time object.

            componenttime(year, month, day, hour, minute, second)

        Parameters
        ----------
            year:
            month:  default = 1
            day: default = 1
            hour: default = 0
            minute: default = 0
            second: default = 0

        Returns
        -------
            Component Time class object

        ::
            >>> timeStart = cdtime.comptime(2017)
            >>> timeEnd = cdtime.comptime(2017,12,31,23,59,59)
            >>> timeStart
            2017-1-1 0:0:0.0
            >>> timeEnd
            2017-12-31 23:59:59.0

    """

    return _cdtime.comptime(year, month, day, hour, minute, second)


def r2c(reltime, calendar=_cdtime.GregorianCalendar):
    """
        Convert Relative time object into component time object.
                r2c( reltime [, calendar])

        Parameters
        ----------

            reltime:  Relative time object
            calendar:  Default is Gregorian

        Returns
        -------

            A component time object

        ::
            >>> import cdtime
            >>> timeStart = cdtime.reltime(9, "days since 2008")
            >>> cdtime.r2c(timeStart, cdtime.GregorianCalendar)
            2008-1-10 0:0:0.0
    """
    return _cdtime.r2c(reltime, calendar)


def r2r(reltime, newunits):
    """
        Convert Relative time object to newunits.
                r2r( reltime, newunits )

        Parameters
        ----------

            reltime:  Relative time object
            newunits:  [seconds,hours,days,yeears] since [year]

        Returns
        -------

            A relative time object

        ::
            >>> import cdtime
            >>> timeStart = cdtime.reltime(9, "days since 2008")
            >>> cdtime.r2r(timeStart, "days since 2007")
            374.000000 days since 2007
    """
    return _cdtime.r2r(reltime, newunits)


def relativetime(value, units):
    """
        Create a retlative time object.

            relativetime(value, units)

        Parameters
        ----------
            values:  number (can be floating point)
            units:  [seconds,hours,days,yeears] since [year]

        Returns
        -------
            Relative Time class object

        ::
            >>> timeStart = cdtime.reltime(9, "days since 2008")
            >>> timeStart
            9.000000 days since 2008

    """
    return _cdtime.relativetime(value, units)


def reltime(value, units):
    """
        Create a retlative time object.

            relativetime(value, units)

        Parameters
        ----------
            values:  number (can be floating point)
            units:  [seconds,hours,days,yeears] since [year]

        Returns
        -------
            Relative Time class object

        ::
            >>> timeStart = cdtime.reltime(9, "days since 2008")
            >>> timeStart
            9.000000 days since 2008

    """
    return _cdtime.reltime(value, units)


def s2c(string, calendar=_cdtime.GregorianCalendar):
    """
        Convert a string to a component time object
            s2c(string[, calendar])

        Parameters
        ----------
            string: date string in form "YYYY-MM-DD HH:MM:SS.Milisecond
            calendar:  Default to Gregorian Calendar

        Returns
        -------

            Component time object

        ::
            >>> import cdtime
            >>> startTime = cdtime.s2c("2008-10-01 23:59:59")
            >>> startTime
            2008-10-1 23:59:59.0
            >>> type(startTime)
            comptime

    """
    return _cdtime.s2c(string, calendar)


def s2r(ctime, runits, calendar=_cdtime.DefaultCalendar):
    """
        Convert a string to a relative time object
            s2r(string, relunits [, calendar])

        Parameters
        ----------
            string: date string in form "YYYY-MM-DD HH:MM:SS.Milisecond
            calendar:  Default to Gregorian Calendar

        Returns
        -------
            Relative time object

        ::
            >>> import cdtime
            >>> startTime = cdtime.s2r("2008-10-01 23:59:59","days since 2008")
            >>> startTime
            274.999988 days since 2008
            >>> type(startTime)
            reltime


    """
    return _cdtime.s2r(ctime, runits, calendar)
