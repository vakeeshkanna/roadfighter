//! handle lists of things
class PUBLISH ListClass:public om
{
public:
  ListClass (ListClass * from = NULL);
  ~ListClass ();
  ListClass *AppendItem (void *);
  ListClass *InsertBefore (void *);
  ListClass *InsertAfter (void *);
  ListClass *DeleteItem ();
  ListClass *RemoveItem ();
  ListClass *Down ();
  ListClass *Up ();
  Logical Tail ()
  {
    return (forward == NULL);
  };
  Logical Head ()
  {
    return (backward == NULL);
  };
  void GetItemData (void **odata);
  void SetItemData (void *idata);
protected:
    ListClass * backward;
    ListClass *forward;
private:
  void *data;
Int32 _Dirty_Fix_does_nothing_;
};
#endif