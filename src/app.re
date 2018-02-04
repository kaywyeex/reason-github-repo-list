type state = {repoData: option(array(RepoData.repo))};

type action =
  | Loaded(array(RepoData.repo));

let component = ReasonReact.reducerComponent("App");

let style = ReactDOMRe.Style.make(~color="red", ~fontSize="12px", ());

let str = ReasonReact.stringToElement;

let make = _children => {
  ...component,
  initialState: () => {repoData: None},
  didMount: self => {
    let handleReposLoaded = self.reduce(repoData => Loaded(repoData));
    RepoData.fetchRepos()
      |> Js.Promise.(
         then_(repoData => {
           handleReposLoaded(repoData);
           resolve();
         })
       )
      |> ignore;

    ReasonReact.NoUpdate;
  },
  reducer: (action, _state) =>
    switch action {
    | Loaded(loadedRepo) => ReasonReact.Update({repoData: Some(loadedRepo)})
    },
  render: self => {
    let repoItems =
      switch self.state.repoData {
      | Some(repos) =>
        ReasonReact.arrayToElement(
          Array.map(
            (repo: RepoData.repo) => <RepoItem key=repo.full_name repo />,
            repos
          )
        )
      | None => ReasonReact.stringToElement("Loading...")
      };
    <div style className="App">
      <h1> (ReasonReact.stringToElement("Reason Projects")) </h1>
      repoItems
    </div>;
  }
};